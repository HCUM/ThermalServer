/******************************************************************************
 * Copyright (c) 2012-2017 All Rights Reserved, http://www.evocortex.com      *
 *  Evocortex GmbH                                                            *
 *  Emilienstr. 1                                                             *
 *  90489 Nürnberg                                                            *
 *  Germany                                                                   *
 *                                                                            *
 * Contributors:                                                              *
 *  Initial version for Linux 64-Bit platform supported by Fraunhofer IPA,    *
 *  http://www.ipa.fraunhofer.de                                              *
 *****************************************************************************/

#ifndef _IRFILEWRITER_H_
#define _IRFILEWRITER_H_

#include <fstream>
#include <stdint.h>
#include <time.h>
#include "IRImager.h"

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __IRIMAGER_API__ __declspec(dllexport)
#else
#define __IRIMAGER_API__ __declspec(dllimport) 
#endif
#else
#define __IRIMAGER_API__
#endif

namespace evo {

/**
 * @class IRFileWriter
 * @brief Class for interfacing file system. It creates monolithic chunks having all frames concatenated.
 * @author Stefan May
 * @date 4.10.2016
 */
    class __IRIMAGER_API__ IRFileWriter {
    public:

        /**
         * Constructor
         * @param timeStart starting time of recording
         * @param baseDir base directory for writing
         * @param header file header with camera specific information
         */
        IRFileWriter(time_t timeStart, const char *baseDir, RawdataHeader header);

        /**
         * Destructor
         */
        virtual ~IRFileWriter();

        /**
         * Open file stream
         * @return success
         */
        virtual bool open();

        /**
         * Signals whether file has been opened successfully
         * @return file open state
         */
        virtual bool isOpen();

        /**
         * Check if write operations are possible for passed basedir (Constructor)
         * @return success==write operations are possible
         */
        bool canDoWriteOperations();

        /**
         * Write image data to file
         * @param[in] timestamp capture time of image
         * @param[in] img image data
         * @param[in] size size of image data
         * @param[in] nmea NMEA string (GPS string format)
         */
        void write(double timestamp, unsigned char *img, uint32_t chunk, uint32_t size, char nmea[GPSBUFFERSIZE]);

        /**
         * Get start of writing, i.e., capture time of first image in sequence
         * @return image capture time
         */
        time_t getStartTime();

    private:

        /**
         * Open new chunk
         */
        void splitFile();

        bool _isOpen;

        std::ofstream *_f;
        std::ofstream *_fTimestamp;

        time_t _tStart;

        char _filename[256];
        char _filenameTS[256];

        RawdataHeader _header;

        uint32_t _chunk;

    };

} // namespace

#endif

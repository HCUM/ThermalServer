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

#ifndef _IRFILEREADER_H_
#define _IRFILEREADER_H_

#include <fstream>
#include <stdint.h>
#include <time.h>
#include "IRImager.h"
#include "FileControlDevice.h"

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
 * @class IRFileReader
 * @brief Class for interfacing file system. It reads files written with IRFileWriter.
 * @author Stefan May
 * @date 9.10.2016
 */
    class __IRIMAGER_API__ IRFileReader : IRImagerClient {
    public:

        /**
         * Constructor
         * @param[in] fileBase base name of chunk files
         * @param[in] params camera parameter set
         */
        IRFileReader(std::string fileBase, IRDeviceParams &params);

        /**
         * Destructor
         */
        virtual ~IRFileReader();

        /**
         * Signals whether file has been opened successfully
         * @return file open state
         */
        virtual bool isReady();

        /**
         * Get size of raw buffer
         * @return size of raw buffer in bytes
         */
        unsigned int getRawBufferSize();

        /**
         * Get width of raw images
         * @return raw image width
         */
        unsigned int getWidth();

        /**
         * Get height of raw images
         * @return raw image height
         */
        unsigned int getHeight();

        /**
         * Get frequency of raw data stream (fps)
         * @return frames per second
         */
        unsigned int getFrequency();

        /**
         * Get serial number of recorded device
         * @return serial number
         */
        unsigned int getSerial();

        /**
         * Read next frame
         * @return success, false==end of file reached
         */
        bool nextFrame();

        /**
         * Set callback function to be called for new frames
         * @param[in] callback Pointer to callback function for thermal channel
         */
        void setFrameCallback(fptrOptrisFrame callback);

        /**
         * Set calback function to be called when the flag state changes
         * @param[in] callback Pointer to callback function
         */
        void setFlagStateCallback(fptrOptrisFlagState callback);

    private:

        /**
         * Read raw image data
         * @param[out] rawBuffer read buffer. It must be allocated outside. The needed size can be queried with getRawBufferSize.
         * @return success
         */
        bool read(unsigned char *rawBuffer, char nmea[GPSBUFFERSIZE]);

        /**
         * Reset read state. The file will be read again from the beginning.
         */
        void reset();

        /**
         * Derived method from IRImagerClient (see related documentation)
         */
        void onThermalFrame(unsigned short *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg);

        /**
         * Derived method from IRImagerClient (see related documentation)
         */
        void onVisibleFrame(unsigned char *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg);

        /**
         * Derived method from IRImagerClient (see related documentation)
         */
        void onFlagStateChange(unsigned int flagstate, void *arg);

        /**
         * Derived method from IRImagerClient (see related documentation)
         */
        void onRawFrame(unsigned char *data, int size);

        bool _isReady;

        std::ifstream *_f;

        std::string *_filename;

        RawdataHeader _header;

        uint32_t _chunk;

        uint32_t _rawBufferSize;

        unsigned char *_rawBuffer;

        fptrOptrisFrame _cbFrame;
        fptrOptrisFlagState _cbFlag;

        bool _initialized;

        IRImager _imager;
    };

} // namespace

#endif

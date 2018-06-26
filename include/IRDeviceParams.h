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

#pragma once

#include "unicode.h"

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __IRDEVICEPARAMS_API__ __declspec(dllexport)
#else
#define __IRDEVICEPARAMS_API__ __declspec(dllimport) 
#endif
#else
#define __IRDEVICEPARAMS_API__
#endif

namespace evo {

    enum EnumFlagState {
        irFlagOpen, irFlagClose, irFlagOpening, irFlagClosing, irFlagError
    };

/**
 * @struct IRFrameMetadata
 * @brief Structure containing meta data acquired from the PI imager
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    struct IRFrameMetadata {
        unsigned short size;      /*!< Size of this structure */
        unsigned int counter;     /*!< Consecutively numbered for each received frame */
        unsigned int counterHW;   /*!< Hardware counter received from device, multiply with value returned by IRImager::getAvgTimePerFrame() to get a hardware timestamp */
        long long timestamp;      /*!< Time stamp in UNITS (10000000 per second) */
        long long timestampMedia;
        EnumFlagState flagState;  /*!< State of shutter flag at capturing time */
        float tempChip;           /*!< Chip temperature */
        float tempFlag;           /*!< Shutter flag temperature */
        float tempBox;            /*!< Temperature inside camera housing */
        unsigned short pifIn[2];
    };

    enum EnumControlInterface {
        HIDController = 1, UVCController = 2
    };
    enum EnumOutputMode {
        Energy = 1, Temperature = 2
    };

/**
 * @struct IRDeviceParams
 * @brief Structure containing device parameters
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    struct IRDeviceParams {
        unsigned long serial;             /*!< serial number */
        int fov;                          /*!< Field of view */
        Tchar *opticsText;                /*!<  */
        Tchar *formatsPath;               /*!< Path to Format.def file  */
        Tchar *caliPath;                  /*!< Path to calibration files */
        int tMin;                         /*!< Minimum of temperature range */
        int tMax;                         /*!< Maximum of temperature range */
        float framerate;                  /*!< Frame rate */
        EnumControlInterface controller;  /*!< this option is ignored with version >= 2.0, using another controller interface needs the passing of an instance of BaseControlDevice to IRImager::init. */
        int videoFormatIndex;             /*!< Used video format index, if multiple modes are supported by the device, e.g. PI400 format index 0: 32 Hz, 1: 80 Hz. */
        int outputMode;                   /*!< Return energy or temperature data. */
        int bispectral;                   /*!< Use bi-spectral mode, if available (e.g. PI200). */
        int average;                      /*!< Activate average filter, if data stream is subsampled. */
        int autoFlag;                     /*!< Use auto flag procedure. */
        float minInterval;                /*!< Minimum interval for a flag cycle. It defines the time in which a flag cycle is forced at most once.*/
        float maxInterval;                /*!< Maximum interval for a flag cycle. It defines the time in which a flag cycle is forced at least once. */
    };

/**
 * @class IRDeviceParamsReader
 * @brief Helper class for reading PI imager configuration files
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    class __IRDEVICEPARAMS_API__ IRDeviceParamsReader {
    public:

        /**
         * Static xml parsing method
         * @param[in] xmlFile path to xml configuration file
         * @param[out] params imager parameters read from xml file
         */
        static bool readXML(const Tchar *xmlFile, IRDeviceParams &params);

    private:

        /**
         * Constructor
         */
        IRDeviceParamsReader() {};

        /**
         * Destructor
         */
        ~IRDeviceParamsReader() {};
    };

}

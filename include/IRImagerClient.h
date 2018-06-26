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

#ifndef IRIMAGERCLIENT_H
#define IRIMAGERCLIENT_H

#include <iostream>
#include "IRDeviceParams.h"

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
 * @class IRImagerClient
 * @brief Interface for specifying object-oriented frame callback methods
 * @author Stefan May (Evocortex GmbH)
 */
    class __IRIMAGER_API__ IRImagerClient {

    public:

        /**
         * Constructor
         */
        IRImagerClient() {};

        /**
         * Destructor
         */
        virtual ~IRImagerClient() {};

        /**
        * Callback method for raw frame events. The method is called when new data is acquired from device.
        * @param[in] data raw data
        * @param[in] size size of raw data in bytes
        */
        virtual void onRawFrame(unsigned char *data, int size) = 0;

        /**
         * Callback method for thermal frames
         * @param[in] data thermal image
         * @param[in] w width of thermal image
         * @param[in] h height of thermal image
         * @param[in] meta meta data container
         * @param[in] arg user arguments (passed to process method of IRImager class)
         */
        virtual void
        onThermalFrame(unsigned short *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg) = 0;

        /**
         * Callback method for visible frames
         * @param[in] data visible image
         * @param[in] w width of visible image
         * @param[in] h height of visible image
         * @param[in] meta meta data container
         * @param[in] arg user arguments (passed to process method of IRImager class)
         */
        virtual void
        onVisibleFrame(unsigned char *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg) {};

        /**
         * Callback method for flag state events. The method is called when the flag state changes.
         * @param[in] flagstate current flag state
         * @param[in] arg user arguments (passed to process method of IRImager class)
         */
        virtual void onFlagStateChange(unsigned int flagstate, void *arg) {};

    };

}

#endif

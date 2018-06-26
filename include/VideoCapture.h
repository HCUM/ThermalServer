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

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __VIDEOCAPTURE_API__ __declspec(dllexport)
#else
#define __VIDEOCAPTURE_API__ __declspec(dllimport) 
#endif
#else
#define __VIDEOCAPTURE_API__
#endif

#include "IRDeviceDS.h"
#include "IRDeviceParams.h"

namespace evo {

/**
 * @class VideoCapture
 * @brief DirectShow filter graph wrapper.
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    class __VIDEOCAPTURE_API__ VideoCapture {
    public:

        /**
         * Constructor
         */
        VideoCapture();

        /**
         * Destructor
         */
        ~VideoCapture();

        /**
         * Run video capture unit. Call this method after all desired devices have been looked up with the getDevice method.
        **/
        void run();

        /**
         * Get and initialize specific device, configured with IRDeviceParams structure
         * @param[in] params device parameters. A specific device is looked up, if serial is unequal to 0. If serial equals 0, the device found first is returned.
         */
        IRDeviceDS *initializeDevice(IRDeviceParams &params);

    protected:

        void initializeGraph();

        IRDeviceDS *initializeVideo(unsigned long serial, int videoFormatIndex);

    private:

        unsigned long determineSerialNumber(wchar_t *deviceName);

        IFilterGraph2 *_graph;

        ICaptureGraphBuilder2 *_builder;

        IMediaControl *_control;

        IRDeviceDS *_devices;

        int _numDevices;

    };

} // namespace

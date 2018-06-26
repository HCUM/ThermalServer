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
#pragma comment(lib, "strmiids")

#define SKIP_DXTRANS
//#define SHOW_DEBUG_RENDERER
#define _CRT_SECURE_NO_WARNINGS

/* If you are having the problem you can't open dxtrans.h:
 * Open qedit.h and add this to the start of the file:
 *
 * #ifdef SKIP_DXTRANS
 * #define __IDxtAlphaSetter_INTERFACE_DEFINED__
 * #define __IDxtJpeg_INTERFACE_DEFINED__
 * #define __IDxtKey_INTERFACE_DEFINED__
 * #define __IDxtCompositor_INTERFACE_DEFINED__
 * #endif
 *
 * Also replace the line 
 * #include "dxtrans.h"
 * with:
 * #ifndef SKIP_DXTRANS
 * #include "dxtrans.h"
 * #endif
 */

#include <windows.h>
#include <dshow.h>
#include "IRImagerClient.h"

namespace evo {

#ifdef LIBIRIMAGER_EXPORTS
#define __IRDEVICEDS_API__ __declspec(dllexport)
#else
#define __IRDEVICEDS_API__ __declspec(dllimport)
#endif

#ifndef MAXLONGLONG
#define MAXLONGLONG 0x7FFFFFFFFFFFFFFF
#endif

#ifndef MAX_DEVICES
#define MAX_DEVICES 8
#endif

#ifndef MAX_DEVICE_NAME
#define MAX_DEVICE_NAME 80
#endif

#ifndef BITS_PER_PIXEL
#define BITS_PER_PIXEL 16
#endif

    class IRDeviceDS;

    struct ISampleGrabber;

    class CallbackHandler;

    typedef void(*VideoCaptureCallback)(unsigned char *data, int len, int instanceID);

/**
 * @class IRDeviceDS
 * @brief DirectShow device interface (Windows platforms)
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    class __IRDEVICEDS_API__ IRDeviceDS {
    public:

        /**
         * Constructor
         */
        IRDeviceDS();

        /**
         * Destructor
         */
        ~IRDeviceDS();

        /**
         * Get serial number of attached device
         * @return serial number
         */
        unsigned long getSerial();

        /**
         * Get image width
         * @return image width in pixels
         */
        unsigned int getWidth();

        /**
         * Get image height
         * @return image height in pixels
         */
        unsigned int getHeight();

        /**
         * Get device frequency
         * @return frame rate of device [1/s]
         */
        unsigned int getFrequency();

        /**
         * Start video streaming
         * @return success==true
         */
        int startStreaming();

        /**
         * Stop video streaming
         * @return success==true
         */
        int stopStreaming();

        /**
         * Get ID of device. If multiple devices are present, the ID is increased by 1 for each device. The fist device has the ID==1.
         * @return ID
         */
        int getId();

        /**
         * Get readable name of attached device
         * @return name of device type
         */
        const char *getFriendlyName();

        /**
         * Callback for getting access to raw data. It is called from a DirectShow thread.
         * @param[in] cb pointer to callback function
         */
        void setCallback(VideoCaptureCallback cb);

        /**
        * Set callback client
        * @param[in] client pointer to instance of imager client (onRawFrame will be called)
        **/
        void setClient(IRImagerClient *client);

    private:

        void onRawFrame(unsigned char *data, int len);

        int _id;

        unsigned long _serial;

        unsigned int _width;

        unsigned int _height;

        unsigned int _frequency;

        char *_friendlyName;

        WCHAR *_filterName;

        IBaseFilter *_sourceFilter;

        IBaseFilter *_sampleGrabberFilter;

        IBaseFilter *_nullRenderer;

        ISampleGrabber *_sampleGrabber;

        IFilterGraph2 *_graph;

        CallbackHandler *_callbackHandler;

        friend class VideoCapture;

        VideoCaptureCallback _cbOnRawFrame;
    };

} // namespace

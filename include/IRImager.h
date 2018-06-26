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

#ifndef IRIMAGER_H
#define IRIMAGER_H

#include <stdlib.h>
#include "IRDeviceParams.h"
#include "IRImagerClient.h"
#include "unicode.h"

#if(WIN32 & _MSC_VER <= 1600)
typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;
#else

#include <inttypes.h>

#endif


#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __IRIMAGER_API__ __declspec(dllexport)
#else
#define __IRIMAGER_API__ __declspec(dllimport) 
#endif
#else
#define __IRIMAGER_API__
#endif

class BaseControlDevice;

struct NewFrameBuffer;

class ImageProcessing;

namespace evo {

#define GPSBUFFERSIZE 80
#define HEADERVERSION 1001

/**
 * @struct RawdataHeader
 * @brief Meta data structure describing camera parameters of image stream
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
#pragma pack(push, 1)
    struct RawdataHeader {
        uint16_t sizeOfHeader;
        uint16_t headerVersion;
        char sdate[8];
        char stime[6];
        uint32_t serial;
        int32_t width;
        int32_t height;
        uint16_t bitCount;
        int64_t avgTimePerFrame;
        uint16_t optics;               // Optics in ° or mm
        uint16_t tempMinRange;         // minimum temprange
        uint16_t tempMaxRange;         // maximum temprange
        uint16_t hwRev;                // hardware release
        uint16_t fwRev;                // firmware release
        uint16_t gpsStringBufferSize;  // size of the GPS string buffer of each frame
    };
#pragma pack(pop)

/**
 * Callback type for thermal frames
 * @param[in] data thermal data, convert to real temperatures with ((((float)data[i])-1000.f))/10.f
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
    typedef void (*fptrOptrisFrame)(unsigned short *data, unsigned int w, unsigned int h, IRFrameMetadata meta,
                                    void *arg);

/**
 * Callback type for visible frames
 * @param[in] data RGB data
 * @param[in] w width of image
 * @param[in] h height of image
 * @param[in] meta meta data container
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
    typedef void (*fptrOptrisVisibleFrame)(unsigned char *data, unsigned int w, unsigned int h, IRFrameMetadata meta,
                                           void *arg);

/**
 * Callback type for flag state events. A registered function is called when the flag state changes.
 * @param[in] fs flag state
 * @param[in] arg arbitrary user-defined argument (passed to process method)
 */
    typedef void (*fptrOptrisFlagState)(EnumFlagState fs, void *arg);

    class Timer;

/**
 * @class IRImager
 * @brief Wrapper for PI driver and image processing library
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    class __IRIMAGER_API__ IRImager {
    public:

        /**
         * Standard constructor
         */
        IRImager();

        /**
         * Destructor
         */
        ~IRImager();

        /**
         * Initializing routine, to be called after instanciation
         * @param[in] params Device parameters
         * @param[in] frequency Frame rate
         * @param[in] width Raw image width (differs from thermal image)
         * @param[in] height Raw image height (differs from thermal image)
         */
        bool init(IRDeviceParams *params, unsigned int frequency, unsigned int width, unsigned int height,
                  bool realDevice = true, BaseControlDevice *dev = NULL);

        /**
         * Initialize Rawdata header with camera specific parameters
         * @param[out] header Rawdata header
         */
        void initRawdataHeader(RawdataHeader &header);

        /**
         * Reconnect resets internal variables and calls init method again.
         */
        bool reconnect(IRDeviceParams *params, unsigned int frequency, unsigned int width, unsigned int height,
                       bool realDevice = true, BaseControlDevice *dev = NULL);

        /**
         * Check for opened device
         * @return device opened
         */
        //bool isOpen();

        /**
         * Check existence of calibration file set. Class instance must be initialized before.
         * @return missing files as comma separated list
         */
        Tchar *checkCalibration();

        /**
         * Set flag automatic
         * @param[in] autoFlag true=enable automatic, false=disable automatic
         */
        void setAutoFlag(bool autoFlag);

        /**
         * Get flag automatic
         * @return true=automatic enabled, false=automatic disabled
         */
        bool getAutoFlag();

        /**
         * Set temperature range
         * @param[in] tMin minimal temperature, e.g. -20,   0, 150
         * @param[in] tMax maximum temperature, e.g. 100, 250, 900
         */
        bool setTempRange(int tMin, int tMax);

        /**
         * Get hardware revision
         * @return revision number
         */
        unsigned int getHWRevision();

        /**
         * Get firmware revision
         * @return revision number
         */
        unsigned int getFWRevision();

        /**
         * Get libirimager version
         */
        static std::string getVersion();

        /**
         * Get image width of thermal channel
         * @return Image width, i.e. number of columns
         */
        unsigned int getWidth();

        /**
         * Get image height of thermal channel
         * @return Image height, i.e. number of rows
         */
        unsigned int getHeight();

        /**
         * Get image width of visible channel (if available)
         * @return Image width, i.e. number of columns
         */
        unsigned int getVisibleWidth();

        /**
         * Get image height of visible channel (if available)
         * @return Image height, i.e. number of rows
         */
        unsigned int getVisibleHeight();

        /**
         * Get width of raw format (from UVC stream)
         * @return width
         */
        unsigned int getWidthIn();

        /**
         * Get height of raw format (from UVC stream)
         * @return height
         */
        unsigned int getHeightIn();

        /**
         * Get # of bits used for temperature coding
         * @return # of bits
         */
        unsigned short getBitCount();

        /**
         * Get average time per frame
         * @return average time
         */
        long long getAvgTimePerFrame();

        /**
         * Get maximum frame rate of device
         * return frame rate (in frames/second)
         */
        float getMaxFramerate();

        /**
         * Get time interval between hardware frames (camera timestamp)
         * @return time interval in [s]
         */
        float getHWInterval();

        /**
         * Get raw image size (includes meta data)
         * @return Number of bytes
         */
        unsigned int getRawBufferSize();

        /**
         * Check if bispectral technology is available
         * @return bispectral technology flag
         */
        bool hasBispectralTechnology();

        /**
         * Get energy buffer of previously acquired frame
         * @param[out] Output buffer (needs to be allocated outside having the size of getWidth()*getHeight())
         * @return success flag (==0)
         */
        int getEnergyBuffer(unsigned short *&buffer);

        /**
         * Get meta data container of previously acquired frame
         * @param[out] Output buffer
         * @param[in] size Size of buffer in bytes
         * @return number of copied bytes
         */
        int getMetaData(unsigned char *&buffer, int size);

        /**
         * Set callback function to be called for new frames
         * @param[in] callback Pointer to callback function for thermal channel
         */
        void setFrameCallback(fptrOptrisFrame callback);

        /**
         * Set callback function to be called for new frames
         * @param[in] callback Pointer to callback function for visible channel
         */
        void setVisibleFrameCallback(fptrOptrisVisibleFrame callback);

        /**
         * Set callback function to be called for changing flag states
         * @param[in] callback Pointer to callback function for flag state events
         */
        void setFlagStateCallback(fptrOptrisFlagState callback);

        /**
         * Set client as callback receiver
         * @param[in] client callback client
         */
        void setClient(IRImagerClient *client);

        /**
         * Process raw data
         * @param[in] buffer Raw data acquired with getFrame()
         * @param[in] arg User-defined data (will be returned with callback methods)
         */
        void process(unsigned char *buffer, void *arg = NULL);

        /**
         * Force shutter flag event manually (close/open cycle)
         * @param[in] time point of time in future in [ms], when flag should be closed
         */
        void forceFlagEvent(float time = 0.f);

        /**
         * Check if shutter flag is open
         * @return flag open
         */
        bool isFlagOpen();

        /**
         * Get temperature of shutter flag
         * @return temperature
         */
        float getTempFlag();

        /**
         * Get temperature of housing
         * @return temperature
         */
        float getTempBox();

        /**
         * Get temperature of chip
         * @return temperature
         */
        float getTempChip();

        /**
         * Enable heating of bolometers
         * @param[in] enable enable flag
         */
        void enableChipHeating(bool enable);

        /**
         * Get state of bolometers heating
         * @return enable flag
         */
        bool isChipHeatingEnabled();

        /**
         * Set reference input of bolometer heating (limited to +20° - 55°). The chip temperature can be monitored with getTempChip().
         * @param[in] t temperature in °C
         */
        void setTempChipReference(float t);

        /**
         * Get reference input of bolometer heating
         * @return temperature of bolometers in °C
         */
        float getTempChipReference();

        /**
         * Set radiation properties, i.e. emissivity and transmissivity parameters
         * @param[in] emissivity emissivity of observed object [0;1]
         * @param[in] transmissivity transmissivity of observed object [0;1]
         */
        void setRadiationParameters(float emissivity, float transmissivity);

        /**
         * Internal method not to be used by any application
         */
        void onFlagState(unsigned int flagstate);

        /**
         * Internal method not to be used by any application!
         */
        void
        onThermalFrameInit(unsigned int width, unsigned int height, unsigned short bitCount, long long avgTimePerFrame);

        /**
         * Internal method not to be used by any application!
         */
        void onThermalFrame(unsigned short *buffer, IRFrameMetadata meta);

        /**
         * Internal method not to be used by any application!
         */
        void onVisibleFrameInit(unsigned int width, unsigned int height);

        /**
         * Internal method not to be used by any application!
         */
        void onVisibleFrame(unsigned char *buffer, IRFrameMetadata meta);

        /**
         * Internal method to communicate with uvc device
         */
        //int readControl(unsigned int id, int* value);

        /**
         * Internal method to communicate with uvc device
         */
        //int writeControl(unsigned int id, int value);

        /**
         * Serialize image
         * @param[in] filename file name
         * @param[in] preview destination buffer for preview data
         * @param[in] sizePreview size of destination buffer
         */
        int serialize(const char *filename, const char *preview, int sizePreview);

        /**
         * Activate trace module. If the processing chain needs to be debugged, this trace functionality might help. But be careful: Tracing is not thread safe!
         * Do not use it in applications having more than one camera connected.
         * @param[in] activate activation state
         */
        void activateTrace(bool activate);

        /**
         * Print trace record (needs to be activated before)
         * @param[in] pixelID ID of pixel, which should be investigated
         */
        void printTrace(const unsigned int pixelID);

    private:

        void freeMemory();

        void startFlag();

        bool _init;

        IRDeviceParams _params;

        unsigned int _widthIn;

        unsigned int _heightIn;

        unsigned int _widthOut;

        unsigned int _heightOut;

        long long _avgTimePerFrame;

        unsigned short _bitCount;

        unsigned short *_buffer;

        unsigned int _widthOutVisible;

        unsigned int _heightOutVisible;

        unsigned char *_bufferVisible;

        fptrOptrisFrame _cbFrame;

        fptrOptrisVisibleFrame _cbVisibleFrame;

        fptrOptrisFlagState _cbFlag;

        IRImagerClient *_client;

        float _maxFramerate;

        unsigned int _frequency;

        bool _firstFlag;

        float _tBox;

        float _tChip;

        float _tFlag;

        float _emissivity;

        float _transmissivity;

        Timer *_t;

        Timer *_tManual;

        float _timeToManualEvent;

        BaseControlDevice *_udev;

        NewFrameBuffer *_SGBuffer;

        ImageProcessing *_ip;

        unsigned int _instanceID;

        float _hwinterval;

        EnumFlagState _eFlagstate;

        bool _startFlag;
    };

}
#endif

/**
 * \example examples/linux/minimal/serializeRaw.cpp
 * Linux example: This minimal example demonstrates how to acquire raw data from an PI imager and serialize it to disk. Raw data can be imported to PIConnect.
 */

/**
 * \example examples/linux/minimal/convertRaw2PPM.cpp
 * Linux example: This minimal example demonstrates how convert recorded raw data to PPM image files.
 */

/**
 * \example examples/linux/oop/irimagerShowOOP.cpp
 * Linux example: This object-oriented example demonstrates how to acquire data from an PI imager and use callback methods of a derived class.
 */

/**
 * \example examples/linux/oop/IRImagerHandler.cpp
 * Linux example: This object-oriented example demonstrates how to derive a class to receive method callback's when new data is available.
 */

/**
 * \example examples/linux/opengl/irimagerShow.cpp
 * Linux example: This example demonstrates how to acquire data from an PI imager and display it with a lightweight OpenGL viewer.
 */

/**
 * \example examples/linux/opengl/irimagerTwinShow.cpp
 * Linux example: This example demonstrates how to acquire data from two PI imagers simultaneously and display both data streams with a lightweight OpenGL viewer.
 */

/**
 * \example examples/windows/minimal/serializeRaw.cpp
 * Windows example: This minimal example demonstrates how to acquire data from an PI imager and serialize it to disk. Raw data can be imported to PIConnect.
 */

/**
 * \example examples/windows/directshow/irimagerShow.cpp
 * Windows example: This example demonstrates how to acquire data from an PI imager and display it via GDI.
 */

/**
 * \example examples/windows/directshow/irimagerShowOOP.cpp
 * Windows example: This object-oriented example demonstrates how to acquire data from an PI imager and display it via GDI.
 */

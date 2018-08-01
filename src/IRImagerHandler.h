#ifndef OOP_IRIMAGERHANDLER_H_
#define OOP_IRIMAGERHANDLER_H_



#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <queue>

// IR device interfaces
#include "IRDeviceUVC.h"

// IR imager interfaces
#include "IRImager.h"

// Helper class for checking calibration files
#include "IRCalibrationManager.h"

// Logging interface
#include "IRLogger.h"

// Image converter
#include "ImageBuilder.h"

// Visualization
#include "Obvious2D.h"

// Time measurement
#include "Timer.h"






/**
 * Optris PI imager interface
 */
#include "IRImager.h"

/**
 * Optris UVC device interface
 */
#include "IRDeviceUVC.h"

/**
 * Optris image converter
 */
#include "ImageBuilder.h"

/**
 * Optris frame rate calculation helper
 */
#include "FramerateCounter.h"

/**
 * Visualization
 */
#include "Obvious2D.h"


using namespace evo;
using namespace std;

/**
 * @class IRImagerHandler
 * @brief Represents an object-oriented example of how to manage thermal and visible images retrieved from PI imagers
 * @author Stefan May (Evocortex GmbH)
 */
class IRImagerHandler : public IRImagerClient {

public:

    /**
     * Constructor
     * @param device pointer to already instantiated raw video device
     * @param imager pointer to already configured imager instance
     */
    IRImagerHandler(IRDeviceUVC *device, IRImager *imager);

    /**
     * Destructor
     */
    ~IRImagerHandler();

    /**
     * Check if new frame is available
     * @param availability flag
     */
    bool checkForNewFrame();

    /**
     * Overwritten method from IRImagerClient
     */
    void onRawFrame(unsigned char* data, int size) override ;

    /**
     * Overwritten method from IRImagerClient
     */
    void onThermalFrame(unsigned short *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg) override ;


    void onThermalFrameEvent(unsigned short* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg) override;

    /**
     * Overwritten method from IRImagerClient
     */
    void onVisibleFrame(unsigned char *data, unsigned int w, unsigned int h, IRFrameMetadata meta, void *arg) override ;

    void onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h, IRFrameMetadata meta, void* arg) override ;

    //void onFlagStateChange(unsigned int flagstate, void *arg) override {};
    /**
     * Overwritten method from IRImagerClient
     */
    void onFlagStateChange(EnumFlagState flagstate, void* arg) override ;

    /**
     * Get pointer to last retrieved thermal image
     * @return thermal image
     */
    unsigned short *getThermalImage();

    /**
     * Get width of last retrieved thermal image
     * @return image width
     */
    unsigned int getThermalWidth();

    /**
     * Get height of last retrieved thermal image
     * @return image height
     */
    unsigned int getThermalHeight();

    /**
     * Get pointer to last retrieved visible image (only available for PI200 and PI230)
     * @return visible image
     */
    unsigned char *getVisibleImage();

    /**
     * Get width of last retrieved visible image
     * @return image width
     */
    unsigned int getVisibleWidth();

    /**
     * Get height of last retrieved visible image
     * @return image height
     */
    unsigned int getVisibleHeight();

    unsigned int forceFlagEvent();


private:

    IRImager *_imager;
    IRDeviceUVC *_device;
    unsigned char *_bufferRaw;

    unsigned int _wThermal;
    unsigned int _hThermal;
    unsigned int _wVisible;
    unsigned int _hVisible;
    unsigned short *_thermal;
    unsigned char *_yuyv;

    bool _newFrame;

};

#endif // OOP_IRIMAGERHANDLER_H_
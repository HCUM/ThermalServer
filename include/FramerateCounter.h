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

#ifndef FRAMERATECOUNTER_H_
#define FRAMERATECOUNTER_H_

#include <iostream>
#include <list>

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __FRAMERATECOUNTER_API__ __declspec(dllexport)
#else
#define __FRAMERATECOUNTER_API__ __declspec(dllimport) 
#endif
#else
#define __FRAMERATECOUNTER_API__
#endif

namespace evo {

    struct LinkedElement;

    class Timer;

/**
 * @class FramerateCounter
 * @brief Framerate calculation helper
 * @author Stefan May (Evocortex GmbH)
 */
    class __FRAMERATECOUNTER_API__ FramerateCounter {
    public:
        /**
         * Standard constructor
         * @param[in] interval time interval in ms
         * @param[in] smoothSize half size of history, of which mean value is calculated.
         */
        FramerateCounter(double interval = 1000.0, unsigned int smoothSize = 30);

        /**
         * Destructor
         */
        virtual ~FramerateCounter();

        /**
         * Trigger, i.e., integrate new measurement
         * @param[out] fps frames per second
         * @return signal when interval (constructor) is elapsed
         */
        bool trigger(double *fps = NULL);

        /**
         * Accessor to current frames per second rate (no triggering is performed)
         * @return fps
         */
        double getFps();

    private:

        Timer *_tElapsed;

        Timer *_tPrint;

        double _fps;

        double _interval;

        struct LinkedElement *_oldest;

        struct LinkedElement *_newest;

    };

}

#endif /* FRAMERATECOUNTER_H_ */

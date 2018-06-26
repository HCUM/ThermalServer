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

#ifndef IRLOGGER_H
#define IRLOGGER_H

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __IRILOGGER_API__ __declspec(dllexport)
#else
#define __IRILOGGER_API__ __declspec(dllimport)
#endif
#else
#define __IRILOGGER_API__
#endif

#include <string>

namespace evo {

    enum IRLoggerVerbosityLevel {
        IRLOG_OFF = 1, IRLOG_ERROR = 2, IRLOG_WARN = 3, IRLOG_INFO = 4, IRLOG_DEBUG = 5
    };

/**
 * @class IRLogger
 * @brief Logging interface, changes verbosity level of the processing chain
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 */
    class __IRILOGGER_API__ IRLogger {
    public:

        /**
         * Change verbosity level
         * @param[in] levelStdout Verbosity level of messages printed to stdout
         * @param[in] levelFile   Verbosity level of messages printed to file
         * @param[in] filename    Optional: Specify filename, default: piimager.log
         */
        static void
        setVerbosity(IRLoggerVerbosityLevel levelStdout, IRLoggerVerbosityLevel levelFile, const char *filename = NULL);

    private:

        /**
         * Constructor
         */
        IRLogger();

        /**
         * Destructor
         */
        ~IRLogger();

    };

}

#endif

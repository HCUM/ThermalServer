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

#ifndef IRCALIBRATIONMANAGER_H
#define IRCALIBRATIONMANAGER_H

#include <fstream>
#include <string>
#include "unicode.h"
#include <vector>

namespace evo {

#ifdef WIN32
#ifdef LIBIRIMAGER_EXPORTS
#define __IRCALIBRATIONMANAGER_API__ __declspec(dllexport)
#else
#define __IRCALIBRATIONMANAGER_API__ __declspec(dllimport) 
#endif
#else
#define __IRCALIBRATIONMANAGER_API__
#endif

    struct IRTempRange {
        int tMin;
        int tMax;
    };

    struct IROptics {
        int fov;
        std::vector <IRTempRange> tempRanges;
        tstring text;
    };

/**
 * @class IRCalibrationManager
 * @brief Class for checking and downloading calibration files for IRImager devices
 * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
 * @date 19.7.2015
 */
    class __IRCALIBRATIONMANAGER_API__ IRCalibrationManager {

    public:

        /**
         * Destructor
         **/
        ~IRCalibrationManager();

        /**
         * Static singleton instance accessor
         * @return singleton instance of IRCalibrationManager class
         **/
        static IRCalibrationManager *getInstance();

        /**
         * Set calibration directory
         * @param[in] dir full directory path to folder containing calibration files
         **/
        void setCalibrationDir(const Tchar *dir);

        /**
         * Set formats directory
         * @param[in] dir full directory path to folder containing the Formats.def file
         **/
        void setFormatsDir(const Tchar *dir);

        /**
         * Get calibration directory
         * @return full directory path to folder containing calibration files
         **/
        const Tchar *getCalibrationDir();

        /**
         * Get formats directory
         * @return full directory path to folder containing the Formats.def file
         **/
        const Tchar *getFormatsDir();

        /**
         * Check for installed calibration files
         * @param[in] serial Serial number of device
         * @param path Path of calibration files
         */
        Tchar *checkCalibration(unsigned long serial);

        /**
         * Check available optics for a specific device
         * @param[in] serial serial number of device
         * @return vector of available optics (fov)
         */
        const std::vector <IROptics> *getAvailableOptics(unsigned long serial);

        /**
        * Get (concat) path and name of configuration file
        * @param[out] Path of configuration file
        * @param[in] directory Directory of configuration file
        * @param[in] maxLen Maximum character length of path parameter
        * @param[in] fileName Name of configuration file
        * @param[in] extension Extension of configuration file
        */
        bool createConfigFilepath(Tchar *path, const Tchar *directory, short maxLen, const Tchar *fileName,
                                  const Tchar *extension);

        /**
        * Generate XML configuration for a specific device
        * @param[in] serial Serial number of device
        * @return text content of standard configuration file
        */
        tstring generateConfiguration(unsigned long serial);

        /**
         * Check for Internet access to calibration files
         * @return availability flag
         */
        bool isOnlineCalibrationRepoAccessible();

        /**
         * Download calibration files for a specific serial number
         * @param serial Serial number of device
         */
        bool downloadCalibration(unsigned long serial);

        /**
         * ONLY LINUX: Copy calibration files from local device, e.g., USB stick
         * @param[in] serial serial number of device for which calibration files are to be found
         * @param[in] srcDir source directory to search for calibration files
         * @return success of search and copy operation
         */
        bool copyCalibrationFromLocalRepo(unsigned long serial, std::string srcDir);

        /**
        * ONLY LINUX: Determine serial number of attached device
        * @param[in] serial Serial number (pass 0 to search for serial of an attached device, pass serial to search for a specific device).
        * @return success flag
        */
        bool findSerial(unsigned long &query);

    private:

        IRCalibrationManager();

        void init(const Tchar *caliPathDefault, const Tchar *formatsPathDefault);

        static bool downloadTarget(Tchar *listOfMissingFiles, tstring serial, tstring targetDir);

        Tchar *_pathCali;

        Tchar *_pathFormats;

        std::vector <IROptics> *_optics;

        static IRCalibrationManager *_this;
    };

}

#endif // IRCALIBRATIONMANAGER_H

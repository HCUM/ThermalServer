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

#ifndef SIMPLEXML_H
#define SIMPLEXML_H

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <shlobj.h>
#else

#include <stdlib.h>
#include <fstream>

#endif

#include "unicode.h"

using namespace std;

namespace evo {

#define NODES 256

/**
 * @class SimpleXML
 * @brief Simple XML parser
 * @author Matthias Wiedemann (Optris GmbH), Stefan May (Evocortex GmbH)
 */

    class SimpleXML {
    public:

        /**
         * Constructor
         */
        SimpleXML(void);

        /**
         * Destructor
         */
        ~SimpleXML(void);

        /**
         * Open file
         * @param[in] dir directory
         * @param[in] xmlFile file name
         * @param[in] root xml root
         * @return success
         */
        bool open(const Tchar *dir, Tchar *xmlFile, const Tchar *root);

        /**
         * Open file
         * @param[in] filename full file path
         * @param[in] root xml root
         * @return success
         */
        bool open(const Tchar *filename, const Tchar *root);

        /**
         * Set node to root of tree
         * @return
         */
        bool setNode(void);

        /**
         * Set specific node by name
         * @param[in] node node name
         * @return success
         */
        bool setNode(const Tchar *node);

        /**
         * Set specific node by name and index
         * @param[in] node node name
         * @param[in] index n'th node with same name
         * @return success
         */
        bool setNode(const Tchar *node, int index);

        /**
         * Move one node back
         * @return success (false, if first node with same name is already reached)
         */
        bool resetNode(void);

        /**
         * Get number of nodes with same name
         * @param[in] node node name
         * @return number of nodes
         */
        int getNodeCount(const Tchar *node);

        /**
         * Get integer value
         * @param[in] node node tag
         * @param[out] value value
         * @return success
         */
        bool getInt(const Tchar *node, int *value);

        /**
         * Get long integer
         * @param[in] node node tag
         * @param[out] value value
         * @return success
         */
        bool getLong(const Tchar *node, long *value);

        /**
         * Get float value
         * @param[in] node node tag
         * @param[out] value value
         * @return success
         */
        bool getFloat(const Tchar *node, float *value);

        /**
         * Get string
         * @param[in] node node tag
         * @param[out] value value
         * @return success
         */
        bool getString(const Tchar *node, Tchar **value);

    private:

        int getKey(const Tchar *key, int begin, int end, bool closeTag = false);

        int _fileSize;

        int _begin[NODES], _end[NODES];

        unsigned short _nodeIndex;

        Tchar *_data;

    };

} // namespace

#endif // SIMPLEXML_H

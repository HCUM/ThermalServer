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

#include "qedit.h"
#include "IRImagerClient.h"

namespace evo {

    class IRDeviceDS;

    typedef void(*__VideoCaptureCallback__)(unsigned char *data, int len);

    /**
    * @class CallbackHandler
    * @brief Callback handler for DirectShow graph
    * @author Stefan May (Evocortex GmbH), Matthias Wiedemann (Optris GmbH)
    */
    class CallbackHandler : public ISampleGrabberCB {

    public:

        /**
        * Constructor
        * @param parent DirectShow device
        */
        CallbackHandler(IRDeviceDS *parent);

        /**
        * Destructor
        */
        ~CallbackHandler();

        /**
        * Set callback function
        * @param[in] cb pointer to callback function. It is called from SampleCB method.
        */
        void setCallback(__VideoCaptureCallback__ cb);

        /**
         * Set callback client
         * @param[in] client pointer to instance of imager client (onRawFrame will be called)
         **/
        void setClient(IRImagerClient *client);

        /**
        * Implemented method for ISampleGrabberCB interface. See documentation of ISampleGrabberCB for the meaning of parameters.
        */
        virtual HRESULT STDMETHODCALLTYPE

        SampleCB(double time, IMediaSample *sample);

        /**
        * Implemented method for ISampleGrabberCB interface. See documentation of ISampleGrabberCB for the meaning of parameters.
        */
        virtual HRESULT STDMETHODCALLTYPE

        BufferCB(double time, BYTE *buffer, long len);

        /**
        * Implemented method for IUnknown interface. See documentation of IUnknown for the meaning of parameters.
        */
        virtual HRESULT STDMETHODCALLTYPE

        QueryInterface(const IID &iid, LPVOID *ppv);

        /**
        * Implemented method for IUnknown interface. See documentation of IUnknown for the meaning of parameters.
        */
        virtual ULONG STDMETHODCALLTYPE

        AddRef();

        /**
        * Implemented method for IUnknown interface. See documentation of IUnknown for the meaning of parameters.
        */
        virtual ULONG STDMETHODCALLTYPE

        Release();

    private:

        __VideoCaptureCallback__ _callback;

        IRImagerClient *_client;

        IRDeviceDS *_parent;

    };

} //namespace

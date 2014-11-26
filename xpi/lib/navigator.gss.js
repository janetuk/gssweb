/*
 * Copyright (c) 2014, JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

console.log('Loading navigator.gss.js - #9');

/* This file gets injected into the web page verbatim */

var GSSEap = (function () 
  {
    function GSSEap(config)
    {
        // Public attributes
        this.version = "0.0.1";
        this.implemented_methods = ["gss_import_name", "gss_display_name", "gss_init_sec_context", "gss_acquire_cred"];
	// MRW -- combine success/error callback hashes?
        this.callbacks = {};
        this.errors = {};
        this.appTag = config.appTag || "";
        this.default_error = config.error || 
            function (major, minor, error, appTag) {
                console.warn(error);
            };
        window.addEventListener(
            "message",
            this.dispatch_responses.bind(this)
        );
    }
    GSSEap.prototype.dispatch_responses = function (event) 
    {
        var method;
        var nonce;
        var callback;
        var app_tag;
        var error;

        /* This message is destined for us only if all the following apply:
        * - The data.method_name is one of the methods implemented by this
        *   object
        * - data.return_values exists or data.error_mssage exists
        * - data.cookies exists
        * - One of my callbacks matches the nonce in
        *   data.cookies.navigator_gss_tag
        * - One of my methods matches the nonce in 
        *   data.cookies.navigator_gss_tag and that method matches
        *   data.method
        */
        method = event.data.method;
        if (
             ( -1 == this.implemented_methods.indexOf(method) ) ||
             (  ("undefined" == typeof (event.data.return_values) ) &&
                ("undefined" == typeof (event.data.error_message) ) ) ||
             ("undefined" == typeof (event.data.cookies)))
        {
            return;
        }

        nonce = event.data.cookies.navigator_gss_tag;
        event.data.cookies.navigator_gss_tag = undefined;
        callback = this.callbacks[nonce];
        if ("undefined" == typeof (callback)) {
            return;
        }

        // We now know that this message is for us!
        this.callbacks[nonce] = undefined;
        app_tag = event.data.cookies.app_tag;
        error = this.errors[nonce] || this.default_error;

        if ("undefined" != typeof(event.data.error_message) )
        {
            error(-1, -1, "Error parsing message: " + event.data.error_message, app_tag);
        }
        else if (this.gss_error(event.data.return_values.major_status))
        {
            var errMsg = "Error during " + method + ": " + 
              "Major status message: " + 
              event.data.return_values.errors.major_status_message + 
              "; Minor status message: " + 
              event.data.return_values.errors.minor_status_message;
            error(
              event.data.return_values.major_status,
              event.data.return_values.minor_status, 
              errMsg,
              app_tag);
        } else {
            callback(event.data.return_values, app_tag);
        }
    };


    GSSEap.prototype.init_sec_context = function (params) 
    {
        /* variables */
        // internal variables
        var nonce;
        
        // Required parameters
        var target_name = params.target_name;
        var callback = params.success || this.success;

        // Optional & defaulted parameters (some are defaulted at lower layer)
        var context_handle = params.context_handle;
        var cred_handle = params.cred_handle; 
	var mech_type = params.mech_type; 
	var req_flags = params.req_flags;
	var time_req = params.time_req;
	var input_token = params.input_token;

        var error = params.error || this.default_error; 
        var app_tag = params.app_tag || this.appTag;

        /* Error checking */
        // Call an error if we don't have the required parameters.
        // - name
        // - success()
        if ( "undefined" == typeof(target_name) ||
             "undefined" == typeof(callback) )
        {
          error(-1, -1, 
            "init_sec_context called missing either target_name or success callback"
          );
          return;
        }
        
        /* Setup */
        nonce = navigator.generateNonce();

        /* Main processing */
        // Save our callback, method name, and error function
        this.callbacks[nonce] = callback;
        this.errors[nonce] = error;
        
        // Now pass the request on to the C code
        window.postMessage({
            "method":"gss_init_sec_context",
            "arguments":
            {
                "target_name": target_name,
		"context_handle": context_handle,
		"cred_handle": cred_handle,
		"mech_type": mech_type,
		"req_flags": req_flags,
		"time_req": time_req,
		"input_token": input_token
		
            },
            "cookies":
            {
                "navigator_gss_tag": nonce,
                "app_tag": app_tag
            }
        }, "*");
        
    };

    GSSEap.prototype.display_name = function(params)
    {
        /* Variables */
        // required parameters
        var input_name = params.input_name;
        var callback = params.success;

        if ( "undefined" == typeof(name) ||
             "undefined" == typeof(callback) )
        {
          error(-1, -1, 
            "import_name called missing either name or success callback"
          );
          return;
        }

        var error = params.error || this.default_error; 
        var app_tag = params.app_tag || this.appTag;
        
        /* Setup */
        nonce = navigator.generateNonce();


        /* Main processing */
        // Save our callback, method name, and error function
        this.callbacks[nonce] = callback;
        this.errors[nonce] = error;
        
        // Now pass the request on to the C code
        window.postMessage({
            "method":"gss_display_name",
            "arguments":
            {
                "input_name": input_name,
            },
            "cookies":
            {
                "navigator_gss_tag": nonce,
                "app_tag": app_tag
            }
        }, "*");
        
    }

    GSSEap.prototype.import_name = function (params) 
    {
        /* variables */
        // internal variables
        var nonce;
        
        // Required parameters
        var name = params.name;
        var callback = params.success;
        
        // Optional & defaulted parameters
        var name_type = params.name_type || "{1 2 840 113554 1 2 1 4 }";
        var error = params.error || this.default_error; 
        var app_tag = params.app_tag || this.appTag;


        /* Error checking */
        // Call an error if we don't have the required parameters.
        // - name
        // - success()
        if ( "undefined" == typeof(name) ||
             "undefined" == typeof(callback) )
        {
          error(-1, -1, 
            "import_name called missing either name or success callback"
          );
          return;
        }

        
        /* Setup */
        nonce = navigator.generateNonce();


        /* Main processing */
        // Save our callback, method name, and error function
        this.callbacks[nonce] = callback;
        this.errors[nonce] = error;
        
        // Now pass the request on to the C code
        window.postMessage({
            "method":"gss_import_name",
            "arguments":
            {
                "input_name": name,
                "input_name_type": name_type
            },
            "cookies":
            {
                "navigator_gss_tag": nonce,
                "app_tag": app_tag
            }
        }, "*");
        
    };

    GSSEap.prototype.gss_error = function (major) 
    {
        var callingMask;
        var routineMask;
        var mask;

        callingMask = 255 << 24;
        routineMask = 255 << 16;
        mask = callingMask | routineMask;

        return (0 != (major & mask));
    };
    return GSSEap;
})();

navigator.gss_eap = GSSEap;

console.log('Loading navigator.gss.js - #7');

/* This file gets injected into the web page verbatim */



var GSSEap = (function () 
  {
    function GSSEap(config)
    {
        // Public attributes
        this.version = "0.0.1";
        this.callbacks = {};
        this.methods = {};
        this.appTag = config.appTag || "";
        this.error = config.error || function (major, minor, error, appTag) {
            console.warn(error);
        };
        window.addEventListener("message", this.dispatch_responses.bind(this));
    }
    GSSEap.prototype.dispatch_responses = function (event) 
    {
        var method;
        var nonce;
        var callback;
        var app_tag;

        /* This message is destined for us only if all the following apply:
        * - The data.method_name is one of the methods implemented by this
        *   object
        * - data.return_values exists
        * - data.cookies exists
        * - One of my callbacks matches the nonce in data.cookies.navigator_gss_tag
        * - One of my methods matches the nonce in data.cookies.navigator_gss_tag
        *   and that method matches data.method
        */
        method = event.data.method;
        if (
             (method != "gss_import_name") ||
             ("undefined" == typeof (event.data.return_values)) ||
             ("undefined" == typeof (event.data.cookies)))
        {
            return;
        }

        nonce = event.data.cookies.navigator_gss_tag;
        event.data.cookies.navigator_gss_tag = undefined;
        callback = this.callbacks[nonce];
        if ("undefined" == typeof (callback) || this.methods[nonce] != method) {
            return;
        }

        // We now know that this message is for us!
        this.callbacks[nonce] = undefined;
        app_tag = event.data.cookies.app_tag;

        if (this.gss_error(event.data.return_values.major_status))
        {
            var errMsg = "Error during " + method + ": " + 
              "Major status message: " + 
              event.data.return_values.errors.major_status_message + 
              "; Minor status message: " + 
              event.data.return_values.errors.minor_status_message;
            this.error(
              event.data.return_values.major_status,
              event.data.return_values.minor_status, 
              errMsg,
              app_tag);
        } else {
            if ("gss_import_name" == method) 
            { callback(event.data.return_values.gss_name, app_tag); }
            // if( "gss_X" == method ) { callback(event.data.return_values.x, app_tag);}
        }
    };

    GSSEap.prototype.import_name = function (params) 
    {
        /* variables */
        var nonce;
        var name = params.name;
        var name_type = params.name_type || "{1 2 840 113554 1 2 1 4 }";
        var callback = params.success;
        var error = params.error || this.error; 
        var app_tag = params.app_tag || this.appTag;

        /* Erorr checking */
        // Call an error if we don't have the required parameters.
        // - name
        // - success()
        if ( "undefined" == typeof(name) ||
             "undefined" == typeof(success) )
        {
          this.error(-1, -1, 
            "import_name called missing either name or success callback"
          );
          return;
        }
        
        nonce = navigator.generateNonce();
        this.callbacks[nonce] = callback;
        this.methods[nonce] = "gss_import_name";
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

        callingMask = 377 << 24;
        routineMask = 377 << 16;
        mask = callingMask | routineMask;

        return (0 != (major & mask));
    };
    return GSSEap;
})();

navigator.gss_eap = GSSEap;

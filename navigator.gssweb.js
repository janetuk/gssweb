/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 JISC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


var GSSWeb = (function () {
  function GSSWeb(config) {
    this.serverPath = config.serverPath;
    this.credential = config.credential;
    this.appTag = config.appTag || "GSSWeb-" + navigator.generateNonce();
    this.error = config.error || function (err) {
      console.warn(err);
    };
    this.success = config.success;

    this.version = "0.0.1";
    this.authenticationState = false;
    this.context = "";
    this.serverToken = "";
    this.clientCred = "";
    this.xhr = new XMLHttpRequest();

    this.gss = new navigator.gssEap({
      appTag: this.appTag,
      error: this.handleGSSError.bind(this)
    });
  }
  /* What to invoke when the underlying GSS library 
   * has a problem.
   */
  GSSWeb.prototype.handleGSSError = 
    function (major, minor, errMsg, appTag) {
      this.error(errMsg, appTag);
    };

  /* The basic authenticate function.
   * Takes no arguments, as all should be supplied
   * on the base object.
   */
  GSSWeb.prototype.authenticate = function () {
    /* Error checking */
    // Ensure that the callbacks exist well
    if ( "function" != typeof(this.error) )
    {
      // I can't even call my error function!  All that can be done
      // is throw an error.
      throw("Error function not supplied to navigator.gssweb object!");
    }
    if ( "function" != typeof(this.success) )
    {
      // OK, so we have an error function; use it.
      this.error(
        "Success function not supplied to navigator.gssweb object!"
      );
    }
    if ( ! this.presentString(this.serverPath) )
    {
      this.error("Server path not supplied to navigator.gssweb object!");
    }

    
    /* Setup */
    this.nonce = navigator.generateNonce();

    // Start off the cascade by getting the
    // GSS name of the server
    this.authGetServerName();
  };

  GSSWeb.prototype.authGetServerName = function () {
    this.gss.import_name({
      name: "HTTP@" + window.location.hostname,
      success: this.authReceiveServerName.bind(this)
    });
  };
  GSSWeb.prototype.authReceiveServerName = function (data, appTag) {
    this.serverName = data.gss_name;

    /* Either move on to acquire_cred because we have been
    * supplied a credential, or move on to init_sec_context
    * when we have not.
    */
    if ( this.presentString(this.credential) )
      this.authGetClientName();
    else
      this.authInitSecContext();
  };

  GSSWeb.prototype.authGetClientName = function () {
    this.gss.import_name({
      name: this.credential,
      success: this.authReceiveClientName.bind(this)
    });
  };
  GSSWeb.prototype.authReceiveClientName = function (data, appTag) {
    this.clientName = data.gss_name;

    // Next up: Get the local credential
    this.authAcquireCred();
  };

  GSSWeb.prototype.authAcquireCred = function () {
    this.gss.acquire_cred({
      desired_name: this.clientName,
      cred_usage: 1,
      success: this.authReceiveClientCred.bind(this)
    });
  };
  GSSWeb.prototype.authReceiveClientCred =
    function (cred, actual_mechs, lifetime_rec) {
      this.clientCred = cred;

      this.authInitSecContext();
    };

  GSSWeb.prototype.authInitSecContext = function () {
    var params = {
      target_name: this.serverName,
      success: this.sendTokenToServer.bind(this)
    };

    if ("" != this.clientCred) {
      params.cred_handle = this.clientCred;
    }
    if ("" != this.serverToken) {
      params.input_token = this.serverToken;
    }
    if ("" != this.context) {
      params.context_handle = this.context;
    }

    this.gss.init_sec_context(params);
  };

  GSSWeb.prototype.sendTokenToServer = 
    function (data, 
              app_tag) {
    this.clientToken = data.output_token;
    this.context = data.context_handle;

    var msg = "nonce=" + this.nonce +
               "&token=" + encodeURIComponent(this.clientToken);

    this.xhr.open("POST", this.serverPath, true);

    this.xhr.setRequestHeader(
      'Content-Type', 
      'application/x-www-form-urlencoded'
    );
    this.xhr.onreadystatechange = this.recvTokenFromServer.bind(this);

    this.xhr.send(msg);
  };

  GSSWeb.prototype.recvTokenFromServer = function () {
    // Only care when we're ready
    if (this.xhr.readyState != 4) {
      return;
    }

    switch (this.xhr.status) {
      case 200:
        // Finished!
        var serverResponse = JSON.parse(this.xhr.responseText);
        var decoded = window.atob(serverResponse.application.data);
        this.authenticationState = true;
        this.success(
          decoded,
          serverResponse.application["content-type"],
          this.appTag
        );
        break;
      case 401:
        // Continue needed
        var serverResponse = JSON.parse(this.xhr.responseText);
        this.serverToken = serverResponse.gssweb.token;
        this.authInitSecContext();
        break;
      default:
        // We have some server-reported error
        this.error(
          window.location.hostname + 
          " reported an error; aborting",
          this.appTag
        );

        // Destroy the GSS context.
        this.context = undefined;
        return;
    }
  };
  
  /*************************************
   * Utility methods
   *************************************/
  // return true if the variable is a non-empty string
  GSSWeb.prototype.presentString = function(str)
  {
    return(
      "string" == typeof(str) &&
      "" != str
    );
  }
  
  return GSSWeb;
})();

navigator.gssweb = GSSWeb;

console.log('Loading navigator.gss.js - #5');

/* This file gets injected into the web page verbatim */

navigator.gss_callbacks = {};

/*
navigator.generateNonce = function() {
  // TODO: Make sure that we don't have a collision!
  // Random integer in the range [0..(2^32-1)]
  return Math.floor(Math.random() * ( 4294967295 )) ;
}
*/

navigator.gss_import_name = function(name, mech, callbackFn, appTag){
    var nonce = navigator.generateNonce();
    navigator.gss_callbacks[nonce] = callbackFn;
    
    /* Listen for a message back from the content script */
    window.addEventListener(
      "message",
      function(event)
      {
        var app_tag;
        var name;
        var callback;
        var nonce;
        
        if (event.data.method != "gss_import_name" ||
            (typeof(event.data.return_values) == "undefined") )
        {
          return;
        }
        
        var nonce = event.data.cookies.navigator_gss_tag;
        event.data.cookies.navigator_gss_tag = undefined;
        callback = navigator.gss_callbacks[nonce];
        navigator.gss_callbacks[nonce] = undefined;
        
        // Extract the data from the returned JSON
        name = event.data.return_values.gss_name;
        app_tag = event.data.cookies.app_tag;
        major = event.data.return_values.major_status;
        minor = event.data.return_values.minor_status;
        
        // Invoke the callback with the extracted data
        callback(name, major, minor, app_tag);
      }
    );

    /* Send a message off to the extension that we want to 
     * call gss_import_name
     */
    window.postMessage({
	"method":"gss_import_name",
	"arguments":
	{
	    "input_name": name,
	    "input_name_type": mech
	},
        "cookies":
        {
            "navigator_gss_tag": nonce,
            "app_tag": appTag
        }
    }, "*");

};

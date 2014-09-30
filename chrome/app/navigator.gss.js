console.log('Loading navigator.gss.js - #5');

/* This file gets injected into the web page verbatim */

navigator.gss_import_name = function(name, mech, nonce, callbackFn){
    console.log("Name: " + name);
    console.log("Mech: " + mech);
    
    /* Listen for a message back from the content script */
    window.addEventListener(
      "message",
      function(event)
      {
        var nonce;
        var name;
        
        if (event.data.command != "gss_import_name" ||
            (typeof(event.data.return_values) == "undefined") )
        {
          return;
        }
        
        // Extract the data from the returned JSON
        name = event.data.return_values.gss_name;
        nonce = event.data.nonce;
        major = event.data.return_values.major_status;
        minor = event.data.return_values.minor_status;
        
        // Invoke the callback with the extracted data
        callbackFn(name, nonce, major, minor);
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
        "nonce": nonce
    }, "*");

};

console.log("gssweb_background.js loading: #4");




var gssNativePort = null;
var applicationPort = null;

/* What to do with the output of the GSS command line */
function onGSSResponse(msg) {
  console.info('Response from GSS command line: [' + 
	       JSON.stringify(msg) + ']'
	      );
  
  // Send the message on to the content script
  applicationPort.postMessage(msg);
  console.info('Response sent to the content script.');
}

function connectToNativeHost() {
  console.info('Connecting to json_gssapi command line.');
  // var host = 'com.google.chrome.example.echo';
  var host = 'com.painlesssecurity.jsongss';
  gssNativePort = chrome.runtime.connectNative( host );
  gssNativePort.onMessage.addListener( onGSSResponse );
}


connectToNativeHost();

// When we receive a connection from a page through the content script...
chrome.runtime.onConnect.addListener(
  function(thePort) 
  {
    // ... First, make sure that we're talking to the right people
    console.assert(thePort.name == "com.painlesssecurity.gssweb");

    // ... Second, save out the port
    applicationPort = thePort;
    
    applicationPort.onMessage.addListener(
      // Now, when we receive a message
      function(msg)
      {
        console.info(
          'About to send message to Native Port: [' +
          JSON.stringify(msg) + ']'
        );
	gssNativePort.postMessage(msg);
        console.info('... message sent to Native Port.')
	
      }
    );
  }
);


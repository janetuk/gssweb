/*
 * Copyright (c) 2015, JANET(UK)
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
console.log("gssweb_background.js loading: #4");

var gssNativePort = null;
var applicationPorts = {};

navigator.generateNonce = function() { 
  // TODO: Make sure that we don't have a collision! 
  // Random integer in the range [0..(2^32-1)] 
    return Math.floor(Math.random() * ( 4294967295 )) ; 
} 

/* What to do with the output of the GSS command line */
function onGSSResponse(msg) {
  var nonce;
  var appPort;
  var appTag;

  // Read the cookies out of the response  
  if ( typeof(msg.cookies) == 'undefined' ||
       typeof(msg.cookies.gssweb_bg_tag) == 'undefined' )
  {
    console.error(
      "gssweb_background.js received a response from the command-line NativeHost with no gssweb_bg_tag cookie."
    );
    return;    
  }
  appTag = msg.cookies.app_tag;
  nonce = msg.cookies.gssweb_bg_tag;
  msg.cookies.gssweb_bg_tag = undefined;

  // Informationally log
  console.info('[' + appTag + '] Response from GSS command line: [' + 
	       JSON.stringify(msg) + ']'
	      );

  // Find the content script's port that should receive this message  
  appPort = applicationPorts[nonce]
  applicationPorts[nonce] = undefined;
  if ( typeof(appPort) == "undefined")
  {
    console.error(
      "[" + appTag + "] gssweb_background.js received a response from the command-line NativeHost with no associated application port."
    );
    return;    
  }
  // appPort is now guaranteed to exist.


  // Send the message on to the content script
  appPort.postMessage(msg);
  
  console.info('[' + appTag + '] Response sent to the content script.');
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
  function(appPort) 
  {
    // ... First, make sure that we're talking to the right people
    console.assert(appPort.name == "com.painlesssecurity.gssweb");
    
    appPort.onMessage.addListener(
      // Now, when we receive a message
      function(msg)
      {
        var nonce;
        var appTag;

        // Deal with the cookies in the message
        if ( typeof(msg.cookies) == 'undefined')
        {
          msg.cookies = {};
        }
        appTag = msg.cookies.app_tag;

        // Save out the port
        nonce = navigator.generateNonce();
        applicationPorts[nonce] = appPort;
        msg.cookies.gssweb_bg_tag = nonce;
        
        // Send the message to the NativePort / command line
        console.info(
          '[' + appTag + '] About to send message to Native Port: [' +
          JSON.stringify(msg) + ']'
        );
	gssNativePort.postMessage(msg);
        console.info('[' + appTag + '] ... message sent to Native Port.')
	
      }
    );
  }
);


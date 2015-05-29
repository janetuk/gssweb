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
console.log("Loading content script #6...");

function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src", 
                   url   );
  document.head.appendChild(elt);
}

var port;
var browser;

if ("undefined" != typeof(chrome) &&
    "undefined" != typeof(chrome.extension) &&
    "undefined" != typeof(chrome.extension.getURL))
{
  // Running in chrome
  browser = "Chrome"; 
  gss_script_name = chrome.extension.getURL('navigator.gss.js');
  port = chrome.runtime.connect({name: "com.painlesssecurity.gssweb"});
} else {
  // Firefox
  browser = "Firefox";
  gss_script_name = 'chrome://gssweb/content/navigator.gss.js';
}

addScript( gss_script_name );

sendReplyToWebpage = function(gssReplyJSON) {
     var appTag = gssReplyJSON.cookies.app_tag;
     
     console.log("[" + appTag + "] Extension port listener received message: [" + 
                  JSON.stringify(gssReplyJSON) + "]"
		); 
     window.postMessage(gssReplyJSON, "*");
  }


/* When we get a message back from the extension 
 * background script
 */
if ("Chrome" == browser)
{
  port.onMessage.addListener( sendReplyToWebpage );
}
else 
{
  self.port.on('gss_response', sendReplyToWebpage );
}

window.addEventListener("message", function(event) {
    // Check to see if this message's data is data we care about
    if ( typeof(event.data.method) == 'undefined' ||
         typeof(event.data.arguments) == 'undefined' ||
         typeof(event.data.return_values) != 'undefined' )
        return;
    
    if ( typeof(event.data.cookies) == 'undefined' )
    {
      event.data.cookies = {};
    }
    var appTag = event.data.cookies.app_tag;
    
    console.log("[" + appTag + "] Window message listener received message: [" +
		JSON.stringify(event.data) + "]"
		);
    if ("Chrome" == browser)
    {
      port.postMessage(event.data);
    } else 
    {
      self.port.emit("gss_request", event.data);
    }
}, false);


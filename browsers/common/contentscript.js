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
function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src", 
                   url   );
  document.head.appendChild(elt);
}

var port;
var browser;
var gssHostNames = {};
var pendingGssHostNames = {};

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
  var csTag = gssReplyJSON.cookies.cs_tag;

  gssReplyJSON.cookies.cs_tag = undefined;

  /* Save off the hostnames of any reply to GSSImportName if:
   * + There is a csTag
   * + The invoked method was 'gss_import_name'
   * + The method completed successfully
   * + There is a hostname wating in the pendingGssHostNames hash
   *   indexed by csTag
  */
  if ( typeof(csTag) != 'undefined' &&
       gssReplyJSON.method == 'gss_import_name' &&
       typeof(gssReplyJSON.return_values) != 'undefined' &&
       gssReplyJSON.return_values.major_status == '0' &&
       typeof(pendingGssHostNames[csTag]) != 'undefined' )
  {
    gssHostNames[gssReplyJSON.return_values.gss_name] =
      pendingGssHostNames[csTag];
    delete pendingGssHostNames[csTag];
  }

  console.log("[" + appTag +
	      "] Extension port listener received message: [" + 
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
    
    console.log("[" + appTag +
                "] Window message listener received message: [" +
		JSON.stringify(event.data) + "]"
	       );

    /*
     * Deny calls to init_sec_context where we don't know that the
     * target has the same hostname as the origin.
     */
    if(event.data.method == "gss_init_sec_context" &&
       typeof(event.data.arguments) != 'undefined' &&
       gssHostNames[event.data.arguments.target_name] !=
          document.location.hostname)
    {
      console.log("[" + appTag + "] Window message listener received " +
		  "gss_init_sec_context, but the hostname in the " +
		  "target_name could not be found to match the document " +
		  "location hostname.");
      sendReplyToWebpage({
        'method':'gss_init_sec_context',
        'return_values': {
          'major_status': -2,
          'minor_status': -1,
          'major_status_message': 'The GSS call cannot be completed',
          'minor_status_message': 'init_sec_context requires a target ' +
	                          'that matches your page origin.'
        },
        'cookies': event.data.cookies;
      });
      return;
    }

    /* Add a content script tag, csTag */
    var csTag = navigator.generateNonce();
    event.data.cookies.cs_tag = csTag;

    /* Save out the hostname from calls to import_name with an
     * NT hostbased name
     */
    if(event.data.method == 'gss_import_name')
    {
      if( typeof(event.data.arguments) != 'undefined' &&
	  ( event.data.arguments.input_name_type ==
	      "{1 2 840 113554 1 2 1 4 }" ||
	    event.data.arguments.input_name_type ==
	      "1.2.840.113554.1.2.1.4" ) )
      {
	var hostname = /[^@]*$/.exec(event.data.arguments.input_name)[0];
	pendingGssHostNames[csTag] = hostname;
      }
    }

    if ("Chrome" == browser)
    {
      port.postMessage(event.data);
    } else 
    {
      self.port.emit("gss_request", event.data);
    }
}, false);


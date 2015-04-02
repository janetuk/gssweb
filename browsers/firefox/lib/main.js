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

var tabs = require("sdk/tabs");
const {Cu} = require("chrome");

Cu.import("resource://gre/modules/ctypes.jsm");


// TODO: detect which OS we're running, and proactively search for the
//       correct library name.
var json_gssapi;
try {
    json_gssapi = ctypes.open("libjsongssapi.so");
}
catch (e) {
    try {
        json_gssapi = ctypes.open("jsongssapi.dll");
    }
    catch (e) {
        console.log("Error loading the JSON GSS-API library: " + e);
    }
}

var gss_request = function(msg) { return "{\"error_message\":\"JSON GSS-API library was not loaded properly\"}"; }
try {
gss_request = json_gssapi.declare("gss_request",
    ctypes.default_abi,
    ctypes.char.ptr,
    ctypes.char.ptr);
}
catch (e) {
  console.log("Could not find the function 'gss_request' in the JSON GSS-API library: " + e);
}

var self = require("sdk/self");
var data = require("sdk/self").data;
var pageMod = require("sdk/page-mod");

pageMod.PageMod({
  include: "*",
  contentScriptFile: [data.url("contentscript.js")],
  contentScriptWhen: "ready"
});



function invokeNativeGSS(msg)
{
  var appTag;
  var reply;
  var response;
  
  // Deal with the cookies in the message
  if ( typeof(msg.cookies) == 'undefined')
  {
    msg.cookies = {};
  }
  appTag = msg.cookies.app_tag;

  // Send the message to the NativePort / command line
  console.info(
    '[' + appTag + '] About to invoke native function: [' +
    JSON.stringify(msg) + ']'
  );
  reply = gss_request( JSON.stringify(msg) );
  response = JSON.parse(reply.readString() );
  console.info('[' + appTag + '] ... native function invoked.');
  console.info('[' + appTag + '] ... returned: ' + response);
  
  return(response);
}

tabs.on("ready", function(tab) {
  app = tab.attach({ contentScriptFile: data.url("contentscript.js") });
  app.port.on("gss_request", function(message) {
    var response = invokeNativeGSS(message);
    app.port.emit("gss_response", response);
  });
});

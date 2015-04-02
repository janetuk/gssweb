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
  contentScriptFile: [data.url("gssweb.contentscript.js")],
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
  app = tab.attach({ contentScriptFile: data.url("gssweb.contentscript.js") });
  app.port.on("gss_request", function(message) {
    var response = invokeNativeGSS(message);
    app.port.emit("gss_response", response);
  });
});

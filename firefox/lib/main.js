var tabs = require("sdk/tabs");
const {Cu} = require("chrome");

for (i = 0; i < 10; i++) {
  console.log("");
}

console.log("About to load ctypes");
Cu.import("resource://gre/modules/ctypes.jsm");
console.log("  ... loaded.");

var json_gssapi;


// TODO: detect which OS we're running, and proactively search for the
//       correct library name.
console.log("About to load libjsongssapi");
try {
    json_gssapi = ctypes.open("libjsongssapi.so");
}
catch (e) {
    try {
        json_gssapi = ctypes.open("libjsongssapi.dll");
    }
    catch (e) {
        console.log("  ... " + e);
    }
}
console.log("  ... loaded.");

console.log("Declaring the call_gss function:");
const gss_request = json_gssapi.declare("gss_request",
    ctypes.default_abi,
    ctypes.char.ptr,
    ctypes.char.ptr);
console.log("  ... declared.");

/*
console.log("Calling import_name");
var reply;
reply = call_gss("{\"method\":\"gss_import_name\",\"arguments\":{\"input_name\":\"HTTP@localhost\",\"input_name_type\":\"{1 2 840 113554 1 2 1 4 }\"}}");
console.log("  ... Reply: " + reply.readString());
*/


var self = require("sdk/self");
var data = require("sdk/self").data;
var pageMod = require("sdk/page-mod");

pageMod.PageMod({
  include: "*",
  contentScriptFile: [data.url("navigator.gss.js")],
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
  app = tab.attach({ contentScriptFile: data.url("navigator.gss.js") });
  app.port.on("gss_request", function(message) {
    var response = invokeNativeGSS(message);
    app.port.emit("gss_response", response);
  });
});

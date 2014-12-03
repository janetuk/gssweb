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
const call_gss = json_gssapi.declare("gss_request",
    ctypes.default_abi,
    ctypes.char.ptr,
    ctypes.char.ptr);
console.log("  ... declared.");

console.log("Calling import_name");
var reply;
reply = call_gss("{\"method\":\"gss_import_name\",\"arguments\":{\"input_name\":\"HTTP@localhost\",\"input_name_type\":\"{1 2 840 113554 1 2 1 4 }\"}}");
console.log("  ... Reply: " + reply.readString());



var data = require("sdk/self").data;
var pageMod = require("sdk/page-mod");

pageMod.PageMod({
  include: "*",
  contentScriptFile: [data.url("navigator.gss.js")],
  contentScriptWhen: "ready"
});

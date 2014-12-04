console.log("Loading content script #6...");


function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src", 
                   url   );
  document.head.appendChild(elt);
}

var port;

if ("undefined" != typeof(chrome) &&
    "undefined" != typeof(chrome.extension) &&
    "undefined" != typeof(chrome.extension.getURL))
{
  // Running in chrome
  gss_script_name = chrome.extension.getURL('navigator.gssEap.js');
  port = chrome.runtime.connect({name: "com.painlesssecurity.gssweb"});
} else {
  // Firefox
  gss_script_name = 'chrome://gssweb/content/navigator.gssEap.js';
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
if ("undefined" != typeof(port))
{
  port.onMessage.addListener( sendReplyToWebpage );
}
if ("undefined" != typeof(self) &&
    "undefined" != typeof(self.port) )
{
  self.port.on('gss_response', sendReplyToWebpage );
}



window.addEventListener("message", function(event) {
    // We only accept messages from ourselves
//    if ("undefined" != typeof(unsafeWindow) && event.source != unsafeWindow ) // Firefox
//       ("undefined" == typeof(unsafeWindow) && event.source != window)        // Chrome
//	return;
    
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
    if ("undefined" != typeof(port) )
    {
      port.postMessage(event.data);
    } else if ("undefined" != typeof(self) && 
               "undefined" != typeof(self.port) )
    {
      self.port.emit("gss_request", event.data);
    }
}, false);


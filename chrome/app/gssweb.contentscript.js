console.log("Loading content script #6...");


function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src", 
                   url   );
  document.head.appendChild(elt);
}

addScript( chrome.extension.getURL('gssweb_utils.js') );
addScript( chrome.extension.getURL('navigator.gss.js') );

var port = chrome.runtime.connect({name: "com.painlesssecurity.gssweb"});

/* When we get a message back from the extension 
 * background script
 */
port.onMessage.addListener(
  function(gssReplyJSON) {
     var appTag = gssReplyJSON.cookies.app_tag;
     
     console.log("[" + appTag + "] Extension port listener received message: [" + 
                  JSON.stringify(gssReplyJSON) + "]"
		); 
     window.postMessage(gssReplyJSON, "*");
  }
);

window.addEventListener("message", function(event) {
    // We only accept messages from ourselves
    if (event.source != window)
	return;
    
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
    port.postMessage(event.data);
}, false);


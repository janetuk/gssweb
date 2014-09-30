console.log("Loading content script #6...");

var elt = document.createElement("script");
elt.setAttribute("src", 
                 chrome.extension.getURL('navigator.gss.js')
                );
document.head.appendChild(elt);


var port = chrome.runtime.connect({name: "com.painlesssecurity.gssweb"});

/* When we get a message back from the extension 
 * background script
 */
port.onMessage.addListener(
  function(gssReplyJSON) {
     console.log("Extension port listener received message: [" + 
                  JSON.stringify(gssReplyJSON) + "]"
		); 
     window.postMessage(gssReplyJSON, "*");
  }
);

window.addEventListener("message", function(event) {
    // We only accept messages from ourselves
    if (event.source != window)
	return;

    console.log("Window message listener received message: [" +
		JSON.stringify(event.data) + "]"
		);
    port.postMessage(event.data);
}, false);
    

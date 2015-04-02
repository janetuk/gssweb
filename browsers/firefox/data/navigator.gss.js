console.log("Bob is here. #2");

function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src",   url );
  elt.setAttribute('type', 'text/javascript');
  document.body.appendChild(elt);
}

addScript('chrome://gssweb/content/nav.gss.js');

self.port.on('alert', function(message) {
  console.log("Content script received message: " + message);
  self.port.emit("gss_request", "Message back from the content script");
});


var msg = {
  'method': 'gss_import_name',
  'arguments': {
    'input_name': 'HTTP@localhost',
    'input_name_type': "{1 2 840 113554 1 2 1 4 }"
  },
  'cookies': {
      'app_tag': 'Test request'
  }
};

self.port.on('gss_response', function(message) {
  console.log("Content script received a reply from gss_import_name: " + JSON.stringify(message) );
});
self.port.emit("gss_request", msg);

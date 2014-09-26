

var gssAppPort = null;

function report(msg) {
  var response = document.getElementById('response');
  response.innerHTML = '<p>' + msg + '</p>' + response.innerHTML;
}

function onGSSResponse(msg) {
  report('Response from GSS: ' + JSON.stringify(msg) );
}

function connectToNativeHost() {
  report('Connecting to json_gssapi command line.');
  // var host = 'com.google.chrome.example.echo';
  var host = 'com.painlesssecurity.jsongss'
  gssAppPort = chrome.runtime.connectNative( host );
  if (gssAppPort) {report("Connected.");};
  gssAppPort.onMessage.addListener( onGSSResponse );
}

function sendNativeMessage() {
  var msg = JSON.parse( document.getElementById('gss_command').value );
  gssAppPort.postMessage(msg);
  report('Sent message: ' + msg);
}


document.addEventListener('DOMContentLoaded', function () {
  document.getElementById('connect').addEventListener(
      'click', connectToNativeHost);
  document.getElementById('send-message').addEventListener(
      'click', sendNativeMessage);
});

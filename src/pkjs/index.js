Pebble.addEventListener("ready", function(e) {
  console.log("Ready for interaction");
  
  var message = {"JS_READY": 1};
  console.log("Sending messge " + JSON.stringify(message));
  Pebble.sendAppMessage(
    message, 
    function(e) {
      console.log('Message sent: ' + JSON.stringify(e));
    },
    function(e) {
      console.log('Message failed: ' + JSON.stringify(e));
    });
});

Pebble.addEventListener('appmessage', function(e) {
  console.log("Received message " + JSON.stringify(e));
  console.log("Message payload " + JSON.stringify(e.payload));
  
  sendRequest(e.payload.RC_KEY);
});

var tvIP = ''; // TODO move to settings 
var url = 'http://' + tvIP + ':1925/1/input/key';

function sendRequest(key) {
  var keyMessage = {"key": key};
  var request = new XMLHttpRequest();
  console.log("Will send message " + JSON.stringify(keyMessage) + " to url " + url + " via requst " + request);
  
  request.onload = function() {
    console.log('LOAD: Got response: ' + this.responseText);
  };
  request.onerror = function() {
    console.log('ERROR: Got response: ' + this.responseText);
  };
  request.onreadystatechange = function() {
    console.log('CHANGE: Got response: ' + this.responseText);
  };
  request.open('POST', url);
  request.setRequestHeader('Accept', 'application/json');
  request.setRequestHeader('Content-Type', 'application/json');
  request.send(JSON.stringify(keyMessage));
}
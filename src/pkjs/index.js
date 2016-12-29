Pebble.addEventListener("ready", function(e) {
  console.log("Ready for interaction");
  
  var message = {"JS_READY": 1};
  console.log("Sending message " + JSON.stringify(message));
  Pebble.sendAppMessage(
    message, 
    function(e) {
      console.log('Message sent, got response: ' + JSON.stringify(e));
    },
    function(e) {
      console.log('Message failed, got response: ' + JSON.stringify(e));
    });
});

Pebble.addEventListener('appmessage', function(e) {
  console.log("Received message " + JSON.stringify(e));
  console.log("Message payload " + JSON.stringify(e.payload));
  
  var command = e.payload.REQ_COMMAND;
  
  if (command == "POST_INPUT_KEY") {
    sendRequest('POST', 'input/key', e.payload.REQ_DATA);
  } else if (command == "GET_AUDIO_VOLUME") {
    sendRequest('GET', 'audio/volume', null);
  } else {
    console.log('No chance to send message to API: ' + command);
  }
});

var tvIP = ''; // TODO move to settings 

function create_url(ip, apiMethod) {
  return 'http://' + ip + ':1925/1/' + apiMethod;
}

function sendRequest(httpMethod, apiMethod, data) {
  var request = new XMLHttpRequest();
  var url = create_url(tvIP, apiMethod);

  var dataMessage = {};
  if (data) {
    var dataMessage = {"key": data};
  }
  console.log("Will send " + httpMethod + " message " + JSON.stringify(dataMessage) + " to url " + url + " via requst " + request);
  
  request.onload = function() {
    console.log('LOAD: Got response: ' + this.responseText);
  };
  request.onerror = function() {
    console.log('ERROR: Got response: ' + this.responseText);
  };
  request.onreadystatechange = function() {
    console.log('CHANGE: Got response: ' + this.responseText);
  };
  request.open(httpMethod, url);
  request.setRequestHeader('Accept', 'application/json');
  request.setRequestHeader('Content-Type', 'application/json');
  request.send(JSON.stringify(dataMessage));
}
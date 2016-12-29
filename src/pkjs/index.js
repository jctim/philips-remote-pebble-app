var Clay = require('pebble-clay');
var clayConfig = require('./config.json');
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }
  var dict = clay.getSettings(e.response);
  console.log(JSON.stringify(dict));
  // // Send settings values to watch side
  // Pebble.sendAppMessage(dict, function(e) {
  //   console.log('Sent config data to Pebble');
  // }, function(e) {
  //   console.log('Failed to send config data!');
  //   console.log(JSON.stringify(e));
  // });
  
  var currentTvIpAddress = dict['10001'];
  localStorage.setItem('TV_IP_ADDRESS', currentTvIpAddress);
});

var DEFAULT_TV_IP_ADDRESS = '192.168.1.98';

function getStorageValue(item){
    var retVal = localStorage.getItem(item);
    if (retVal == null || retVal == 'undefined' || retVal == 'null'){
        retVal = DEFAULT_TV_IP_ADDRESS;
    }
    return retVal;
}

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

function create_url(apiMethod) {
  return 'http://' + getStorageValue('TV_IP_ADDRESS') + ':1925/1/' + apiMethod;
}

function sendRequest(httpMethod, apiMethod, data) {
  var request = new XMLHttpRequest();
  var url = create_url(apiMethod);

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
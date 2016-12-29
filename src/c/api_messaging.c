#include <pebble.h>
#include "api_messaging.h"

bool _js_ready;

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "received message");
  
  // look for JS_READY message
  Tuple *t = dict_find(iter, MESSAGE_KEY_JS_READY);
  if (t) {
    _js_ready = true;
  }
  
  // look for other messages
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "dropped message");
}

static void outbox_sent_callback(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sent message");
}

static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "failed message");
}


void api_messaging_create() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "registering messaging");
  app_message_open(64, 256);
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
}

void api_messaging_destroy() {
  app_message_deregister_callbacks();
}

void send_api_message_with_callback(char *command, char *data, void *callback) {
  if (!_js_ready) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "JS is not ready yet. Try later");
  }
  DictionaryIterator *iter;
  AppMessageResult result = app_message_outbox_begin(&iter);
  if (result == APP_MSG_OK) {
    // construct
    dict_write_cstring(iter, MESSAGE_KEY_REQ_COMMAND, command);
    dict_write_cstring(iter, MESSAGE_KEY_REQ_DATA, data);
    result = app_message_outbox_send();
    if (result != APP_MSG_OK) {    
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int) result);
    }
    // TODO register the callback
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox: %d", (int) result);
  }
}

void send_api_message(char *command, char *data) {
  send_api_message_with_callback(command, data, NULL);
}
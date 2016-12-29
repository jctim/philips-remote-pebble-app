#include <pebble.h>
#include "api_messaging.h"

bool _js_ready;

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "received message, dict_size=%d", (int)dict_size(iter));
  
  Tuple *t = dict_read_first(iter);
  while(t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "NEXT tuple t(%d,%s)", (int)t->key, t->value->data);

    // TODO look for JS_READY message    
    _js_ready = true;

    // TODO look for other messages
    
    t = dict_read_next(iter);
  }
  
  // look for other messages

  APP_LOG(APP_LOG_LEVEL_DEBUG, "after: received message, dict_size=%d", (int)dict_size(iter));  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "dropped message, reason=%d", reason);
}

static void outbox_sent_callback(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sent message, dict_size=%d", (int)dict_size(iter));
}

static void outbox_failed_callback(DictionaryIterator *iter, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "failed message, reason=%d, dict_size=%d", reason, (int)dict_size(iter));
  
  // TODO just to understand what the hack is going on
  // Tuple *t = dict_read_first(iter);
  // while(t) {
  //   APP_LOG(APP_LOG_LEVEL_DEBUG, "NEXT tuple t(%d,%s)", (int)t->key, t->value->data);
  //   t = dict_read_next(iter);
  // }

  // TODO re-send message???
  
}


void api_messaging_create() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "registering messaging");
      
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "opening messaging");
  app_message_open(256, 256);
}

void api_messaging_destroy() {
  app_message_deregister_callbacks();
}

void send_api_message_with_callback(char *command, char *data, void *callback) {
  if (!_js_ready) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "JS is not ready yet. Try later");
    return;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Start sending message %s...", command);
  DictionaryIterator *iter;
  AppMessageResult result = app_message_outbox_begin(&iter);
  if (result == APP_MSG_OK) {
    // construct
    dict_write_cstring(iter, MESSAGE_KEY_REQ_COMMAND, command);
    dict_write_cstring(iter, MESSAGE_KEY_REQ_DATA, data);
    //dict_write_end(iter);
    result = app_message_outbox_send();
    if (result != APP_MSG_OK) {    
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: result=%d", (int) result);
    } else {
      // TODO + register the callback 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Finish sending message %s!", command);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox: result=%d", (int) result);
  }
}

void send_api_message(char *command, char *data) {
  send_api_message_with_callback(command, data, NULL);
}
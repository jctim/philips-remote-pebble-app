#include "main_window.h"

Window *main_window;
ActionBarLayer *action_bar_layer;

GBitmap *vol_up_button;
GBitmap *vol_down_button;
GBitmap *play_button;

bool js_ready;

// TODO move this initialization to splash screen
static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "received message");
  Tuple *t = dict_find(iter, MESSAGE_KEY_JS_READY);
  if (t) {
    js_ready = true;
  }
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

void send_message(char *data) {
  DictionaryIterator *iter;
  AppMessageResult result = app_message_outbox_begin(&iter);
  if (result == APP_MSG_OK) {
    // construct
    dict_write_cstring(iter, MESSAGE_KEY_RC_KEY, data);
    result = app_message_outbox_send();
    if (result != APP_MSG_OK) {    
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int) result);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing outbox: %d", (int) result);
  }
}

void register_messaging() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "registering messaging");
  app_message_open(64, 256);
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
}

void deregister_messaging() {
  app_message_deregister_callbacks();
}

void volume_up_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "volume up pressed");
  if (js_ready) {
    send_message("VolumeUp");
  }
}

void volume_down_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "volume down pressed");  
  if (js_ready) {
    send_message("VolumeDown");
  }
}

void play_pause_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "play/pause pressed");
  if (js_ready) {
    send_message("PlayPause");
  }
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) volume_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) volume_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) play_pause_click_handler);
}

void setup_action_bar_layer(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "action bar setup");
  
  //Layer *window_layer = window_get_root_layer(window);
  
  action_bar_layer = action_bar_layer_create();
  //action_bar_layer_set
  
  
  vol_up_button = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VOLUME_UP);
  vol_down_button = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VOLUME_DOWN);
  play_button = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY);
  
  action_bar_layer_set_icon_animated(action_bar_layer, BUTTON_ID_UP, vol_up_button, true);
  action_bar_layer_set_icon_animated(action_bar_layer, BUTTON_ID_DOWN, vol_down_button, true);
  action_bar_layer_set_icon_animated(action_bar_layer, BUTTON_ID_SELECT, play_button, true);
  
  action_bar_layer_set_click_config_provider(action_bar_layer, click_config_provider);

  
  //layer_add_child(window_layer, action_bar_layer_get_layer(action_bar_layer));
  action_bar_layer_add_to_window(action_bar_layer, window);
}

void main_window_load(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main load");
  
  setup_action_bar_layer(window);
}

void main_window_unload(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main unload");
  
  action_bar_layer_destroy(action_bar_layer);
  gbitmap_destroy(vol_up_button);
  gbitmap_destroy(vol_down_button);
  gbitmap_destroy(play_button);
}

void main_window_create() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main create");
  
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // TODO Where to move this?
  register_messaging();
}

void main_window_destroy() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main destroy");
  
  deregister_messaging();
  window_destroy(main_window);
}

Window *main_window_get_window() {
  return main_window;
}

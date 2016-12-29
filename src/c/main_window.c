#include "main_window.h"
#include "api_messaging.h"

Window *main_window;
ActionBarLayer *action_bar_layer;

GBitmap *vol_up_button;
GBitmap *vol_down_button;
GBitmap *play_button;

TextLayer *volume_text_layer;

void volume_up_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "volume up pressed");
  send_api_message("POST_INPUT_KEY", "VolumeUp");
}

void volume_down_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "volume down pressed");  
  send_api_message("POST_INPUT_KEY", "VolumeDown");
}

void play_pause_click_handler() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "play/pause pressed");
  send_api_message("POST_INPUT_KEY", "PlayPause");
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

void setup_volume_text(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  volume_text_layer = text_layer_create(GRect(0, 120, bounds.size.w - ACTION_BAR_WIDTH, 48));
  text_layer_set_text(volume_text_layer, "Volume: ");
  text_layer_set_text_alignment(volume_text_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(volume_text_layer));
  
  // TODO update in a cycle
  send_api_message_with_callback("GET_AUDIO_VOLUME", NULL, NULL /* TODO callback for getting result*/);
}

void main_window_load(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main load");
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // volume title
  setup_volume_text(window);
  // action bar
  setup_action_bar_layer(window);
}

void main_window_unload(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main unload");
  
  action_bar_layer_destroy(action_bar_layer);
  gbitmap_destroy(vol_up_button);
  gbitmap_destroy(vol_down_button);
  gbitmap_destroy(play_button);
  text_layer_destroy(volume_text_layer);
}

void main_window_create() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main create");
  
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
}

void main_window_destroy() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main destroy");
  
  window_destroy(main_window);
}

Window *main_window_get_window() {
  return main_window;
}

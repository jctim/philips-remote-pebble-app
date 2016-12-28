#include "main_window.h"

Window *main_window;
ActionBarLayer *action_bar_layer;

GBitmap *vol_up_button;
GBitmap *vol_down_button;
GBitmap *play_button;

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
  
  //action_bar_layer_set_click_config_provider(action_bar, click_config_provider);

  
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
  
}

void main_window_destroy() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main destroy");
  
  window_destroy(main_window);
}

Window *main_window_get_window() {
  return main_window;
}

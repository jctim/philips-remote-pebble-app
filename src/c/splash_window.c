#include <pebble.h>
#include "splash_window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_remote_control_small;
static GBitmap *s_res_image_volume_up;
static GBitmap *s_res_image_volume_down;
static GBitmap *s_res_image_pause;
static BitmapLayer *splash_image_layer;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif
  
  s_res_image_remote_control_small = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_REMOTE_CONTROL_SMALL);
  s_res_image_volume_up = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VOLUME_UP);
  s_res_image_volume_down = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VOLUME_DOWN);
  s_res_image_pause = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE);
  // splash_image_layer
  splash_image_layer = bitmap_layer_create(GRect(0, 0, 144, 100));
  bitmap_layer_set_bitmap(splash_image_layer, s_res_image_remote_control_small);
  layer_add_child(window_get_root_layer(s_window), (Layer *)splash_image_layer);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_image_volume_up);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_volume_down);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_pause);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(splash_image_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_image_remote_control_small);
  gbitmap_destroy(s_res_image_volume_up);
  gbitmap_destroy(s_res_image_volume_down);
  gbitmap_destroy(s_res_image_pause);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_splash_window(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_splash_window(void) {
  window_stack_remove(s_window, true);
}

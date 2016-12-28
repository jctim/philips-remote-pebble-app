#include "splash_window.h"

Window *splash_window;
GBitmap *splash_image;
BitmapLayer *splash_image_layer;
TextLayer *splash_title_layer;

void splash_window_load(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "splash load");
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // splash image
  splash_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_REMOTE_CONTROL_SMALL);
  splash_image_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, 120));
    
  bitmap_layer_set_bitmap(splash_image_layer, splash_image);
  bitmap_layer_set_compositing_mode(splash_image_layer, GCompOpSet);
  
  // splash title
  splash_title_layer = text_layer_create(GRect(0, 120, bounds.size.w, 48));
  text_layer_set_text(splash_title_layer, "Philips TV RC");
  text_layer_set_text_alignment(splash_title_layer, GTextAlignmentCenter);
  
  // add child layers
  layer_add_child(window_layer, bitmap_layer_get_layer(splash_image_layer));
  layer_add_child(window_layer, text_layer_get_layer(splash_title_layer));
}

void splash_window_unload(Window *window)  {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "splash unload");
  
  gbitmap_destroy(splash_image);
  bitmap_layer_destroy(splash_image_layer);
  text_layer_destroy(splash_title_layer);
}

void splash_window_create() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "splash create");
  
  splash_window = window_create();
  window_set_window_handlers(splash_window, (WindowHandlers) {
    .load = splash_window_load,
    .unload = splash_window_unload
  });
  
}

void splash_window_destroy() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "splash destroy");
  
  window_destroy(splash_window);
}

Window *splash_window_get_window() {
  return splash_window;
}

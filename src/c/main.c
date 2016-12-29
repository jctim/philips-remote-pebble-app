#include <pebble.h>
#include "splash_window_prev.h"
#include "main_window.h"
#include "error_window.h"
#include "api_messaging.h"

void launch_splash_window() {
  window_stack_push(splash_window_get_window(), true);
}

void launch_main_window() {
  window_stack_push(main_window_get_window(), true);
  window_stack_remove(splash_window_get_window(), false);
}

void init() {
  splash_window_create();
  main_window_create();
  api_messaging_create();
  
  launch_splash_window();
  //app_timer_register(0, launch_splash_window, NULL);
  app_timer_register(1000, launch_main_window, NULL);
}

void deinit() {
  api_messaging_destroy();
  splash_window_destroy();
  main_window_destroy();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
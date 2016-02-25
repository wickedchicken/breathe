#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
time_t init_time = 0;

#define PHASE_LENGTH 5

void update_display(int secs) {
  char *choices[2] = {"in", "out"};
  char *choice = choices[(secs / PHASE_LENGTH) % 2];
  text_layer_set_text(text_layer, choice);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_display(mktime(tick_time) - init_time);
}

void handle_init(void) {
  time(&init_time);
  my_window = window_create();
  Layer *root_layer = window_get_root_layer(my_window);
  
  GRect bounds = layer_get_bounds(root_layer);
  int half_height = (bounds.size.h / 2);
  text_layer = text_layer_create(GRect(1, half_height - 10, bounds.size.w, half_height));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  update_display(0);
  layer_add_child(root_layer, 
                  text_layer_get_layer(text_layer));
  window_stack_push(my_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

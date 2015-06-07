#include <pebble.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
  
#define NUM_CLOCK_TICKS 360
#define PULSE_VAL 4 //1440
#define BREATH 2.4  
  
  static Window *s_main_window;
  static TextLayer *timelbl, *layer360, *layer180, *layer90,*layer240;
  static GPath *s_minute, *s_hour, *s_spider;
  static Layer *s_simple_bg_layer;
  static char buf[10];
  static GRect bounds;
  static GPoint s_center;
  static int pulse, breath;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  int hour = abs(tick_time->tm_hour - 15); //15 is the offset from time_zero
  pulse = ((hour * 60) / 4) + (tick_time->tm_min / 4);
  int minutes = tick_time->tm_min;
  //breath = ((minutes*60)*4)/100;
  breath = 0;
  snprintf(buf, 10, "%d.%d", pulse,breath);
  text_layer_set_text(timelbl, buf);
   
}

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_circle(ctx, s_center, 50);
  float p_spider_x = s_center.x+(50*cos(pulse*3.14/360));
  float p_spider_y = s_center.y+(50*sin(pulse*3.14/360));
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx,GPoint(p_spider_x, p_spider_y), 5);
  
}

  static void window_load(Window *window) {
    
    Layer *window_layer = window_get_root_layer(window);
    bounds = layer_get_bounds(window_layer);
    s_center = grect_center_point(&bounds);
    
    s_simple_bg_layer = layer_create(bounds);
    layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
    layer_add_child(window_layer, s_simple_bg_layer);
    
    // Create and Add to layer hierarchy:
    timelbl = text_layer_create(GRect((bounds.size.w/2)-sizeof(buf), bounds.size.h/2 , bounds.size.w, 30));
    text_layer_set_text(timelbl, buf);
    text_layer_set_background_color(timelbl, GColorBlack);
    text_layer_set_text_color(timelbl, GColorWhite);
    layer_add_child(window_layer, text_layer_get_layer(timelbl));
    
    layer360 = text_layer_create(GRect(bounds.size.w/2-sizeof("360"), 10, 20, 20));
    text_layer_set_text(layer360, "360");
    text_layer_set_background_color(layer360, GColorBlack);
    text_layer_set_text_color(layer360, GColorWhite);
    layer_add_child(window_layer, text_layer_get_layer(layer360));
    
    layer180 = text_layer_create(GRect(bounds.size.w/2-sizeof("180"),140, 20, 20));
    text_layer_set_text(layer180, "180");
    text_layer_set_background_color(layer180, GColorBlack);
    text_layer_set_text_color(layer180, GColorWhite);
    layer_add_child(window_layer, text_layer_get_layer(layer180));
    
    layer90 = text_layer_create(GRect(130, bounds.size.h/2, 20, 20));
    text_layer_set_text(layer90, "90");
    text_layer_set_background_color(layer90, GColorBlack);
    text_layer_set_text_color(layer90, GColorWhite);
    layer_add_child(window_layer, text_layer_get_layer(layer90));
    
    layer240 = text_layer_create(GRect(0, bounds.size.h/2, 20, 20));
    text_layer_set_text(layer240, "240");
    text_layer_set_background_color(layer240, GColorBlack);
    text_layer_set_text_color(layer240, GColorWhite);
    layer_add_child(window_layer, text_layer_get_layer(layer240));
  }

  static void window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(timelbl);

    // Destroy Window
    window_destroy(window);
   }

  static void init(){
    s_main_window = window_create();

    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload
  });

  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
  }

  static void deinit(){
     window_destroy(s_main_window);
    //gpath_destroy(s_spider);
  }

  int main(void) {
    init();
    app_event_loop();
    deinit();
    }
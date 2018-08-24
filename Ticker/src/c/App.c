
#include "Window.h"
#include "TextBox.h"
#include "Timer.h"
#include "DateTime.h"
#include "Index.h"

#include <pebble.h>


Window* s_mainWindow;

TextLayer* s_timeLayer;
TextLayer* s_dateLayer;
TextLayer* s_indexLayer;
TextLayer* s_indexLayer2;
Layer* s_graphLayer;
Layer* s_graphBgLayer;

const int g_updateFreq = 4;  // per hour

TimerWatcher s_watchers[3];

const char* const BIG_FONT = FONT_KEY_ROBOTO_BOLD_SUBSET_49;
const char* const SML_FONT = FONT_KEY_GOTHIC_24_BOLD; //FONT_KEY_ROBOTO_CONDENSED_21;
const char* const TINY_FONT = FONT_KEY_GOTHIC_18_BOLD;

void renderBg(Layer* frame, GContext* ctx)
{
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, GColorDarkGray);
  // lines every 6 hours
  for (int i = 0; i < 136; i += g_updateFreq)
  {
    graphics_draw_line(ctx, GPoint(i,0), GPoint(i,61));
  }
}


void App_mainWindowLoad(Window *window) 
{
  s_timeLayer = TextBox_construct(window,
    GRect(2, 2, 142, 49),
    GColorClear,
    GColorBlack,
    fonts_get_system_font(BIG_FONT),
    GTextAlignmentCenter);
  //TextBox_write(s_timeLayer, "hh:mm");

  // Create date TextLayer
  s_dateLayer = TextBox_construct(window,
    GRect(4, 52, 136, 28),
    GColorClear,
    GColorBlack,
    fonts_get_system_font(SML_FONT),
    GTextAlignmentCenter);
  //TextBox_write(s_dateLayer, "date...");

  // Create another TextLayer
  s_indexLayer = TextBox_construct(window,
    GRect(4, 78, 136, 28),
    GColorClear,
    GColorBlue,
    fonts_get_system_font(SML_FONT),
    GTextAlignmentLeft);
  TextBox_write(s_indexLayer, "BTC");
    
  s_indexLayer2 = TextBox_construct(window,
    GRect(4, 78, 136, 28),
    GColorClear,
    GColorDarkGreen,
    fonts_get_system_font(SML_FONT),
    GTextAlignmentRight);
  TextBox_write(s_indexLayer2, "£?");
    
  s_graphBgLayer = layer_create(GRect(4, 106, 136, 61));
	//layer_set_background_color(s_graphLayer, GColorWhite);
  layer_add_child(window_get_root_layer(window), s_graphBgLayer);
  layer_set_update_proc(s_graphBgLayer, renderBg);
  s_graphLayer = layer_create(GRect(4, 106, 136, 61));
	//layer_set_background_color(s_graphLayer, GColorWhite);
  layer_add_child(window_get_root_layer(window), s_graphLayer);
}

void App_mainWindowUnload(Window *window) 
{  
//  APP_LOG(APP_LOG_LEVEL_DEBUG, __FUNCTION__);
  // Destroy TextLayers
  TextBox_destroy(s_timeLayer);
  TextBox_destroy(s_dateLayer);
  TextBox_destroy(s_indexLayer);
  TextBox_destroy(s_indexLayer2);
  layer_destroy(s_graphLayer);
  layer_destroy(s_graphBgLayer);
  // Destroy Window after the text layers
  Window_destroy(s_mainWindow);
}

void App_init()
{  
  // TextLayers are not initialised until this is called!!
  s_mainWindow = Window_construct((WindowHandlers) {
    .load = App_mainWindowLoad,
    .unload = App_mainWindowUnload
  });
    
  Window_setBGColour(s_mainWindow, GColorWhite);

  // Update date and time
  DateTime_construct(s_dateLayer, s_timeLayer);
  
  s_watchers[0] = DateTime_getDateWatcher();
  s_watchers[1] = DateTime_getTimeWatcher();
  s_watchers[2] = Index_getWatcher();
  Timer_construct(s_watchers, sizeof(s_watchers) / sizeof(s_watchers[0]));
	Index_construct(s_indexLayer, s_indexLayer2, s_graphLayer);
}

// ideally in reverse order to init
void App_uninit() 
{
//  APP_LOG(APP_LOG_LEVEL_DEBUG, __FUNCTION__);
  Index_destroy();
  DateTime_destroy();
  Timer_destroy();
}

int main(void) 
{
  App_init();
  app_event_loop();
  App_uninit();
}

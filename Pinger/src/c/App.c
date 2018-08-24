#include "Timer.h"
#include "Window.h"
#include <pebble.h>

Window* g_mainWindow;
Layer* g_frame;

//const int linelength = 54; 
static const GPoint g_origin = { 72, 84 };
static bool g_wasConnected;
static bool g_isConnected;
static const GColor8 RED = {.argb=(uint8_t)0b11110000};
static const GColor8 GREEN = {.argb=(uint8_t)0b11001100};
static TimerWatcher g_watcher[1];

static unsigned int g_ticker = 0;

void refresh()
{
  g_wasConnected = g_isConnected;
  g_isConnected = bluetooth_connection_service_peek();
	
	if (g_wasConnected && !g_isConnected)
	{
		vibes_short_pulse();
	}
	else if (!g_wasConnected && g_isConnected)
	{
		vibes_double_pulse();
	}

	++g_ticker;
	
//	APP_LOG(APP_LOG_LEVEL_INFO, "refresh %d", s_ticker % 4);
	
	layer_mark_dirty(g_frame);
}

void redraw(Layer* frame, GContext* ctx)
{
//	APP_LOG(APP_LOG_LEVEL_INFO, "redraw");

	graphics_context_set_stroke_color(ctx, g_isConnected ? GREEN : RED);
	graphics_context_set_fill_color(ctx, g_isConnected ? GREEN : RED);
  graphics_context_set_stroke_width(ctx, 4);

	switch(g_ticker % 4)
	{
	case 3:
		graphics_draw_circle(ctx, g_origin, 40);
	case 2:
		graphics_draw_circle(ctx, g_origin, 30);
	case 1:
		graphics_draw_circle(ctx, g_origin, 20);
	case 0:
		graphics_fill_circle(ctx, g_origin, 10);
	}
}


void App_mainWindowLoad(Window *window) 
{
  g_frame = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(g_frame, redraw);
  layer_add_child(window_get_root_layer(window), g_frame);
}

void App_mainWindowUnload(Window *window) 
{  
  // Destroy graphic
  layer_destroy(g_frame);
}

void App_init()
{  
  // TextLayers are not initialised until this is called!!
  g_mainWindow = Window_construct((WindowHandlers) {
    .load = App_mainWindowLoad,
    .unload = App_mainWindowUnload
  });
	Window_setBGColour(g_mainWindow, GColorBlack);

  g_wasConnected = g_isConnected = bluetooth_connection_service_peek();

  g_watcher[0].handler = refresh; 
  g_watcher[0].updateInterval = 1; 
  g_watcher[0].updateUnits = SECOND_UNIT;
  Timer_construct(g_watcher, 1);	
}

// ideally in reverse order to init
void App_uninit() 
{
  Timer_destroy();
  Window_destroy(g_mainWindow);    
}

int main(void) 
{
  App_init();
  app_event_loop();
  App_uninit();
}

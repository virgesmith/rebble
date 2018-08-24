#include "Conway.h"
#include "Window.h"
#include "TextBox.h"
#include "Timer.h"
#include "DateTime.h"
#include <pebble.h>

Window* g_mainWindow;
Layer* g_frame;
static bool g_wasConnected;
static bool g_isConnected;
TextLayer* g_time;
TimerWatcher g_watchers[2];

static int g_updatePeriod = 12; // in 25ths of second

const char* const TIME_FONT = FONT_KEY_LECO_42_NUMBERS;

static const GColor8 connOkColour = {.argb=(uint8_t)0b11111111};
static const GColor8 noConnColour = {.argb=(uint8_t)0b11110101};


void checkConnection()
{
  g_wasConnected = g_isConnected;
  g_isConnected = bluetooth_connection_service_peek();
	
	if (g_wasConnected && !g_isConnected)
	{
		TextBox_setFGColour(g_time, noConnColour);
		vibes_short_pulse();
	}
	else if (!g_wasConnected && g_isConnected)
	{
		TextBox_setFGColour(g_time, connOkColour);
		vibes_double_pulse();
	}	
}


void evolveHandler(AccelData *data, uint32_t nSamples)
{
	static int i = 0;
	if (i % g_updatePeriod == 0)
		evolve();
	++i;
}

void tapHandler(AccelAxisType axis, int32_t direction)
{
	//if (axis == ACCEL_AXIS_Z)
	//APP_LOG(APP_LOG_LEVEL_INFO, "tap z %d", (int)direction);
	setRandomGlider();
}

void batteryHandler(BatteryChargeState batteryState)
{
	// slow evolution to 1/sec if battery is low
  if (batteryState.charge_percent <= 30)
  {
    g_updatePeriod = 25;   
  }
	else
	{
		g_updatePeriod = 12;
	}
}


void App_mainWindowLoad(Window* window) 
{
  initialise();

  // create graphic
  g_frame = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(g_frame, update);
  layer_add_child(window_get_root_layer(window), g_frame);

	g_wasConnected = g_isConnected = bluetooth_connection_service_peek();

  g_time = TextBox_construct(window,
    GRect(2, 54, 140, 49),
    GColorClear,
    g_isConnected ? connOkColour : noConnColour,
    fonts_get_system_font(TIME_FONT),
    GTextAlignmentCenter);
  TextBox_write(g_time, "hh:mm");

}

void App_mainWindowUnload(Window *window) 
{  
  // Destroy graphic
  layer_destroy(g_frame);
  TextBox_destroy(g_time);
}

void App_init()                   
{  
  // TextLayers are not initialised until this is called!!
  g_mainWindow = Window_construct((WindowHandlers) {
    .load = App_mainWindowLoad,
    .unload = App_mainWindowUnload
  });
	Window_setBGColour(g_mainWindow, GColorBlack);

  DateTime_construct(0, g_time);
  	
  g_watchers[0].handler = checkConnection; 
  g_watchers[0].updateInterval = 10; 
  g_watchers[0].updateUnits = SECOND_UNIT;
  g_watchers[1] = DateTime_getTimeWatcher();
	
  Timer_construct(g_watchers, 2);
	accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ); // doesnt work?		
  accel_data_service_subscribe(1, evolveHandler);	
	accel_tap_service_subscribe(tapHandler);

  batteryHandler(battery_state_service_peek());
  // then set callback for when status chages
  battery_state_service_subscribe(batteryHandler);

}

// ideally in reverse order to init
void App_uninit() 
{
  Window_destroy(g_mainWindow);    
  DateTime_destroy();
}

int main(void) 
{
  App_init();
  app_event_loop();
  App_uninit();
}

#include "GravBall.h"
#include "Window.h"
#include "Accelerometer.h"
#include "TextBox.h"
#include "Timer.h"
#include "DateTime.h"
#include <pebble.h>

Window* g_mainWindow;
Layer* g_frame;
TextLayer* g_time;
TextLayer* g_date;
TimerWatcher g_watchers[3];

//static char s_textbuf[32];
const char* const TIME_FONT = FONT_KEY_ROBOTO_BOLD_SUBSET_49;
const char* const DATE_FONT = FONT_KEY_GOTHIC_24_BOLD;

static const GColor8 connOkColour = {.argb=(uint8_t)0b11000111};
static const GColor8 noConnColour = {.argb=(uint8_t)0b11101010};
static const GColor8 battHighColour = {.argb=(uint8_t)0b11001100};
static const GColor8 battMedColour = {.argb=(uint8_t)0b11111100};
static const GColor8 battLowColour = {.argb=(uint8_t)0b11110000};
static const GColor8 textColour = {.argb=(uint8_t)0b11111111};

static float g_pX[2] = { 72.0, 72.0 };
static float g_pY[2] = { 42.0, 126.0 };
static BallInfo g_balls[2];


void checkConnection()
{
	g_balls[0].colour = bluetooth_connection_service_peek() ? connOkColour : noConnColour;	
}

void checkBattery(BatteryChargeState batteryState)
{
  if (batteryState.charge_percent <= 20)
  {
    g_balls[1].colour = battLowColour;
  }
  else if (batteryState.charge_percent <= 40)
  {
    g_balls[1].colour = battMedColour;   
  }
  else
  {
    g_balls[1].colour = battHighColour;   
  }
	g_balls[1].radius = 10 + batteryState.charge_percent / 10;
}

void collisionHandler(int i, int j)
{
	int tmp = g_balls[i].grav;
	g_balls[i].grav = g_balls[j].grav;
	g_balls[j].grav = tmp;
}

void App_mainWindowLoad(Window* window) 
{
  g_time = TextBox_construct(window,
    GRect(2, 48, 140, 49),
    GColorClear,
    textColour,
    fonts_get_system_font(TIME_FONT),
    GTextAlignmentCenter);
		
  g_date = TextBox_construct(window,
    GRect(2, 100, 140, 25),
    GColorClear,
    textColour,
    fonts_get_system_font(DATE_FONT),
    GTextAlignmentCenter);
  TextBox_write(g_time, "dd mmm yyyy");

  // create graphic
  g_frame = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(g_frame, GravBall_update);
  layer_add_child(window_get_root_layer(window), g_frame);

	g_balls[0] = (BallInfo){.radius = 20, .grav = 1, .style=OUTLINE };
	g_balls[1] = (BallInfo){.radius = 20, .grav = 0, .style=OUTLINE };

	// these set ball colours
	checkConnection();
	checkBattery(battery_state_service_peek());

  Accelerometer_construct(ACCEL_SAMPLING_25HZ, 1, g_frame);
	
	GravBall_construct(layer_get_bounds(g_frame), 2, g_balls, g_pX, g_pY, collisionHandler);
}

void App_mainWindowUnload(Window *window) 
{  
  layer_destroy(g_frame);
  TextBox_destroy(g_time);
  TextBox_destroy(g_date);
	
	GravBall_destroy();
}

void App_init()                   
{  
  // TextLayers are not initialised until this is called!!
  g_mainWindow = Window_construct((WindowHandlers) {
    .load = App_mainWindowLoad,
    .unload = App_mainWindowUnload
  });
	Window_setBGColour(g_mainWindow, GColorBlack);

  DateTime_construct(g_date, g_time);
  	
  g_watchers[0] = DateTime_getDateWatcher(); 
  g_watchers[1] = DateTime_getTimeWatcher();
  g_watchers[2].handler = checkConnection; 
  g_watchers[2].updateInterval = 10; 
  g_watchers[2].updateUnits = SECOND_UNIT;
  Timer_construct(g_watchers, 3);

  battery_state_service_subscribe(checkBattery);
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

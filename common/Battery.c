
#include "Battery.h"

static BitmapLayer* s_display;

static GBitmap* p_battLevelL;
static GBitmap* p_battLevelH;
static GBitmap* p_battLevelF;
static GBitmap* p_battLevelC;

void Battery_handler(BatteryChargeState batteryState)
{
  GBitmap* battLevel;

  if (batteryState.is_charging)
  {
    battLevel = p_battLevelC; 
  }
  else if (batteryState.charge_percent < 25)
  {
    battLevel = p_battLevelL;   
  }
  else if (batteryState.charge_percent < 75)
  {
    battLevel = p_battLevelH;   
  }
  else
  {
    battLevel = p_battLevelF;     
  }

  bitmap_layer_set_bitmap(s_display, battLevel);  
}

void Battery_construct(BitmapLayer* display, int iconIds[4])
{
  p_battLevelL = gbitmap_create_with_resource(iconIds[0]/*RESOURCE_ID_BATTERY_L_ICON*/);
  p_battLevelH = gbitmap_create_with_resource(iconIds[1]/*RESOURCE_ID_BATTERY_H_ICON*/);
  p_battLevelF = gbitmap_create_with_resource(iconIds[2]/*RESOURCE_ID_BATTERY_F_ICON*/);
  p_battLevelC = gbitmap_create_with_resource(iconIds[3]/*RESOURCE_ID_BATTERY_C_ICON*/);

  s_display = display;
  //bitmap_layer_set_background_color(s_display, GColorWhite);
  //bitmap_layer_set_compositing_mode(s_display, GCompOpSet);
  // get and display current status
  Battery_handler(battery_state_service_peek());
  // then set callback when status chages
  battery_state_service_subscribe(Battery_handler);
}

void Battery_destroy()
{
//  APP_LOG(APP_LOG_LEVEL_DEBUG, __FUNCTION__);
  battery_state_service_unsubscribe();
  s_display = 0;  
  gbitmap_destroy(p_battLevelL);
  gbitmap_destroy(p_battLevelH);
  gbitmap_destroy(p_battLevelF);
  gbitmap_destroy(p_battLevelC);  
}


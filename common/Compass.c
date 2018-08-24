// Compass functionality

#include "Compass.h"
#include "TextBox.h"
#include <pebble.h>

extern TextLayer* s_text;
extern Layer* s_frame;
// private
static int p_heading;
static CompassStatus p_status;

void Compass_handler(CompassHeadingData data)
{  
  p_heading = /*TRIGANGLE_TO_DEG*/(data.magnetic_heading);

  p_status = data.compass_status;
   
	//APP_LOG(APP_LOG_LEVEL_INFO, "status %d", p_status);
	 
  layer_mark_dirty(s_frame);
}

int Compass_heading()
{
  return p_heading;
}

CompassStatus Compass_status()
{
  return p_status;
}


const char* Compass_direction()
{
  int quadrant = ((p_heading + TRIG_MAX_ANGLE / 16) % TRIG_MAX_ANGLE) / (TRIG_MAX_ANGLE / 8);
  switch (quadrant)
  {
  case 0: return "N";
  case 1: return "NW";
  case 2: return "W";
  case 3: return "SW";
  case 4: return "S";
  case 5: return "SE";
  case 6: return "E";
  case 7: return "NE";
  default:return "??";
  }  
}

void Compass_construct(int angularSensDegrees)
{
  compass_service_set_heading_filter(angularSensDegrees * TRIG_MAX_ANGLE / 360.0);

  // then set the callback
  compass_service_subscribe(Compass_handler);
}

void Compass_destroy()
{
  compass_service_unsubscribe();
}

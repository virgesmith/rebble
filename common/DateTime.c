
#include "DateTime.h"
#include "TextBox.h"

static TextLayer* s_timeDisplay;
static TextLayer* s_dateDisplay;


void DateTime_dateHandler()
{
  if (!s_dateDisplay) return;
//  APP_LOG(APP_LOG_LEVEL_INFO, "DateTime_dateHandler()");
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char dbuffer[] = "ddd DD mmm";

  strftime(dbuffer, sizeof(dbuffer), "%a %e %b", tick_time);
  text_layer_set_text(s_dateDisplay, dbuffer);
}

void DateTime_timeHandler()
{
  if (!s_timeDisplay) return;
//  APP_LOG(APP_LOG_LEVEL_INFO, "DateTime_timeHandler()");
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char tbuffer[] = "hh:mm";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(tbuffer, sizeof(tbuffer), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(tbuffer, sizeof(tbuffer), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_timeDisplay, tbuffer);
}

void DateTime_secondsHandler()
{
  //APP_LOG(APP_LOG_LEVEL_INFO, "DateTime_secondsHandler()");
  if (!s_timeDisplay) return;
  // Get a tm structure
  time_t temp = time(NULL);
  int secs = 59 - (temp % 60);

  // Create a long-lived buffer
  static char tbuffer[] = ":ss";

  // Write the current hours and minutes into the buffer
  snprintf(tbuffer, sizeof(tbuffer), ":%02d", secs);
  
  if (secs < 10)
  {
    TextBox_setFGColour(s_timeDisplay, GColorRed);
  }
  else
  {
    TextBox_setFGColour(s_timeDisplay, GColorWhite);  
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_timeDisplay, tbuffer);
}


void DateTime_construct(TextLayer* dateLayer, TextLayer* timeLayer)
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "DateTime_construct()");
  s_timeDisplay = timeLayer;
  s_dateDisplay = dateLayer;
  // Make sure the date and time is displayed from the start
  DateTime_dateHandler();
  DateTime_timeHandler();
}

void DateTime_destroy()
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "DateTime_destroy()");
}

TimerWatcher DateTime_getDateWatcher()
{
  return (TimerWatcher) { .handler = DateTime_dateHandler, 
                                   .updateInterval = 1, 
                                   .updateUnits = DAY_UNIT };
}

TimerWatcher DateTime_getTimeWatcher()
{
  return (TimerWatcher) { .handler = DateTime_timeHandler, 
                                   .updateInterval = 1, 
                                   .updateUnits = MINUTE_UNIT };
}

TimerWatcher DateTime_getSecondsWatcher()
{
  return (TimerWatcher) { .handler = DateTime_secondsHandler, 
                                   .updateInterval = 1, 
                                   .updateUnits = SECOND_UNIT };
}

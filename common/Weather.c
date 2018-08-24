
#include "Weather.h"

#include "TextBox.h"

static TextLayer* p_weatherDisplay;
static TextLayer* p_statusDisplay;
static BitmapLayer* p_connStatusDisplay;
static GBitmap* p_connBitmap;
static GBitmap* p_noConnBitmap;
static const int p_bufferSize; 

void Weather_inboxReceived(DictionaryIterator *iterator, void *context) 
{
  //APP_LOG(APP_LOG_LEVEL_INFO, "Weather_inboxReceived()");
  // Store incoming information
  int temperature = -274;
	int pressure = 0;
	int humidity = -1;
  static char weatherBuffer[32]; //bug?? if this isnt static, nothing gets displayed in the text buffer (yet it gets logged ok)

	bool gotWeather = false;
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) 
  {
    // Which key was received?
    switch(t->key) 
    {
			case KEY_CONNECTION_FAILURE:
				// TODO use an icon
			  bitmap_layer_set_bitmap(p_connStatusDisplay, p_noConnBitmap);
				return;
      case KEY_STATUS:
			  gotWeather = true;
        //text_layer_set_text(s_statusDisplay, t->value->cstring);
				//strcmp(t->value->cstring, "OK") ? TextBox_setFGColour(p_statusDisplay, GColorRed) 
				//                                : TextBox_setFGColour(p_statusDisplay, GColorGreen); 
        break;
      case KEY_TIME:
			  // do nothing
				// skip year (1st 5 chars) to fit
        //text_layer_set_text(s_statusDisplay, t->value->cstring + 5);
        break;
      case KEY_TEMPERATURE:
        temperature = t->value->int32;
        break;
      case KEY_PRESSURE:
        pressure = t->value->int32;
        break;
      case KEY_HUMIDITY:
        humidity = t->value->int32;
        break;
      case KEY_LAN_STATUS:
        text_layer_set_text(p_statusDisplay, t->value->cstring);
				strcmp(t->value->cstring, "OK") ? TextBox_setFGColour(p_statusDisplay, GColorRed) 
				                                : TextBox_setFGColour(p_statusDisplay, GColorGreen); 
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
	if (gotWeather)
	{	
		bitmap_layer_set_bitmap(p_connStatusDisplay, p_connBitmap);		
		snprintf(weatherBuffer, sizeof(weatherBuffer), "%dC %dmb %d ", temperature, pressure, humidity);
		weatherBuffer[strlen(weatherBuffer)-1] = '%';
		text_layer_set_text(p_weatherDisplay, weatherBuffer);
	}
}


void Weather_inboxDropped(AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void Weather_outboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void Weather_outboxSent(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void Weather_handler()
{
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}


void Weather_construct(TextLayer* weatherLayer, TextLayer* statusLayer, BitmapLayer* connStatusLayer, int iconIds[2])
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_construct()");
  // Register callbacks
  app_message_register_inbox_received(Weather_inboxReceived);
  app_message_register_inbox_dropped(Weather_inboxDropped);
  app_message_register_outbox_failed(Weather_outboxFailed);
  app_message_register_outbox_sent(Weather_outboxSent);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  p_weatherDisplay = weatherLayer;
  p_statusDisplay = statusLayer;
  p_connStatusDisplay = connStatusLayer;
  p_connBitmap = gbitmap_create_with_resource(iconIds[0]);
  p_noConnBitmap = gbitmap_create_with_resource(iconIds[1]);
  
  // invoke handler so we have location/weather info from the start
  Weather_handler();
}

void Weather_destroy()
{
  gbitmap_destroy(p_connBitmap);
  gbitmap_destroy(p_noConnBitmap);
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_destroy()");
  app_message_deregister_callbacks();
}

TimerWatcher Weather_getWatcher()
{
  return (TimerWatcher) { .handler = Weather_handler, 
                          .updateInterval = 15, 
                          .updateUnits = MINUTE_UNIT };
}

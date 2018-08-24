
#include "Location.h"

static TextLayer* s_weatherDisplay;
static TextLayer* s_locationDisplay;

static const int s_bufferSize; 

void Location_inboxReceived(DictionaryIterator *iterator, void *context) 
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_inboxReceived()");
  // Store incoming information
  int temperature = -274;
  char* conditions = 0;
  static char weatherBuffer[32]; //bug?? if this isnt static, nothing gets displayed in the text buffer (yet it gets logged ok)
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) 
  {
    // Which key was received?
    switch(t->key) 
    {
      case KEY_LOCATION:
        text_layer_set_text(s_locationDisplay, t->value->cstring);
        break;
      case KEY_TEMPERATURE:
        temperature = (int)t->value->int32;
        break;
      case KEY_CONDITIONS:
        conditions = t->value->cstring;
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
  
  snprintf(weatherBuffer, sizeof(weatherBuffer), "%s %dC", conditions, temperature);
  text_layer_set_text(s_weatherDisplay, weatherBuffer);
}


void Location_inboxDropped(AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void Location_outboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void Location_outboxSent(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void Location_handler()
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_handler()");
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}


void Location_construct(TextLayer* s_locLayer, TextLayer* s_weatherLayer)
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_construct()");
  // Register callbacks
  app_message_register_inbox_received(Location_inboxReceived);
  app_message_register_inbox_dropped(Location_inboxDropped);
  app_message_register_outbox_failed(Location_outboxFailed);
  app_message_register_outbox_sent(Location_outboxSent);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  s_weatherDisplay = s_weatherLayer;
  s_locationDisplay = s_locLayer;
  
  // invoke handler so we have location/weather info from the start
  //Location_handler();
}

void Location_destroy()
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_destroy()");
  app_message_deregister_callbacks();
}

TimerWatcher Location_getWatcher()
{
  return (TimerWatcher) { .handler = Location_handler, 
                          .updateInterval = 1, 
                          .updateUnits = HOUR_UNIT };
}

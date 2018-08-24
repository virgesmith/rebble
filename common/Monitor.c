
#include "Monitor.h"

#include "TextBox.h"

static DeviceInfo* p_devices;
static int p_nDevices; 

void populate(enum Keys key, const char* string, int maxLen)
{
	int idx0 = 0;
	int idx1 = 0;
	int i = 0;
	
	do
	{
		while (string[idx1] != ';')
		{
			++idx1;
		}
		if (strncmp(string + idx0, "undefined", 9))	
			strncpy(p_devices[i].data[key], string + idx0, (idx1-idx0) < maxLen -1 ? (idx1 -idx0) : maxLen - 1);
		idx0 = idx1 + 1;
		if (string[idx1] == ';') ++idx1;
		++i;
	} 
	while (string[idx1] && i < p_nDevices);
}


void Monitor_inboxReceived(DictionaryIterator *iterator, void *context) 
{
    // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) 
  {
		// Which key was received?
    switch(t->key) 
    {
			case KEY_NAMES:
				//if (window_stack_get_top_window) == 
				window_stack_pop(true);
			  populate(t->key, t->value->cstring, 32);
				break;
      case KEY_IPS:
			case KEY_OSVERS:
			case KEY_UPTIMES:
			case KEY_LOADS:
			case KEY_MEMS:
			case KEY_DISKS:	
			case KEY_TEMPS:	
			  populate(t->key, t->value->cstring, VALUE_SIZE);
			  break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
}


void Monitor_inboxDropped(AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void Monitor_outboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void Monitor_outboxSent(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void Monitor_handler()
{
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}


void Monitor_construct(DeviceInfo* devices, int nDevices)
{
  p_nDevices = nDevices;
	p_devices = devices;
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_construct()");
  // Register callbacks
  app_message_register_inbox_received(Monitor_inboxReceived);
  app_message_register_inbox_dropped(Monitor_inboxDropped);
  app_message_register_outbox_failed(Monitor_outboxFailed);
  app_message_register_outbox_sent(Monitor_outboxSent);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // invoke handler so we have location/weather info from the start
  Monitor_handler();
}

void Monitor_destroy()
{
  app_message_deregister_callbacks();
}


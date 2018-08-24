
#include "Index.h"
#include "TextBox.h"
#include "utils.h"
#include <pebble.h>

static TextLayer* p_indexDisplay;
static TextLayer* p_indexDisplay2;
static Layer* p_graphDisplay;

static const int BUFFERSIZE = 136; 
static const int INDEX = 137;
//static int* p_bufferIndex; 
//static int* p_buffer;
static const char* p_ccy;

extern const int g_updateFreq;
 

int getBuffer(int i)
{
  return persist_read_int(i);
}

void setBuffer(int i, int v)
{
  persist_write_int(i, v);
}

int getIndex()
{
  return persist_read_int(INDEX);
}

void incIndex()
{
  persist_write_int(INDEX, (getIndex() + 1) % BUFFERSIZE);
}


void Index_handler();

void drawGraph(Layer* frame, GContext* ctx)
{
  // start with the open
	int maxVal = -(1<<30), minVal = +(1<<30);
	// set max/min
	for (int i = 0; i < BUFFERSIZE; ++i)
	{
		int v = getBuffer(i);
	
	  if (v > 0)
		{
			maxVal = utils_max(maxVal, v);
			minVal = utils_min(minVal, v);
		}
	}

	//APP_LOG(APP_LOG_LEVEL_INFO, "drawGraph: (min,max)=(%d,%d)", minVal, maxVal);
	
	int range = maxVal - minVal;
	if (range < 60)
	{
		minVal -= (60 - range) / 2;
		range = 60;
	}
	float scale = 60.0 / range;
	//APP_LOG(APP_LOG_LEVEL_INFO, "r,s=%d,%d", range, (int)scale);
	// set the opening level
	
	//graphics_context_set_stroke_color(ctx, GColorBlue);
	//float	y = 60 + (minVal - p_open) * scale;
	//graphics_draw_line(ctx, GPoint(0,(int)y), GPoint(135,(int)y));
	
	graphics_context_set_stroke_color(ctx, GColorBlack);

	int idx = getIndex();
	int v = getBuffer(idx);

	float y = 60 + (minVal - v) * scale;

	GPoint from = {0, (int)y}; 
	for (int i = 1; i < BUFFERSIZE; ++i)
	{
	  v = getBuffer((idx + i) % BUFFERSIZE);
		y = 60 + (minVal - v) * scale;
		GPoint to = {i, (int)y};
		graphics_draw_line(ctx, from, to);
		from = to;
		//APP_LOG(APP_LOG_LEVEL_INFO, "(x,y)=(%d,%d)", i, (int)y);
	}
}

void Index_inboxReceived(DictionaryIterator *iterator, void *context) 
{
  //APP_LOG(APP_LOG_LEVEL_INFO, "Index_inboxReceived() index = %d", getIndex());

	//static bool firstTime = true;
  //APP_LOG(APP_LOG_LEVEL_INFO, "Weather_inboxReceived()");
  // Store incoming information
  int price = 0;
	//int change = 0;
  static char indexBuffer[32];
  //static char indexBuffer2[32];

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) 
  {
    // Which key was received?
    switch(t->key) 
    {
			case KEY_CONNECTION_STATUS:
				if (t->value->int32 == 1)
				{
					//APP_LOG(APP_LOG_LEVEL_INFO, "Pebble AppMessage ready");
					Index_handler();
          TextBox_setFGColour(p_indexDisplay2, GColorRed);
				}
				return;
      case KEY_PRICE:
			  price = t->value->int32;
				setBuffer(getIndex(), price);
				incIndex();
        //APP_LOG(APP_LOG_LEVEL_INFO, "price = %d", price);
        TextBox_setFGColour(p_indexDisplay2, GColorDarkGreen);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
	snprintf(indexBuffer, sizeof(indexBuffer), "£%d.%02d", price / 100, price % 100);
	text_layer_set_text(p_indexDisplay2, indexBuffer);
		
	layer_mark_dirty(p_graphDisplay);
}


void Index_inboxDropped(AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void Index_outboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void Index_outboxSent(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void Index_handler()
{
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Send name of currency
  dict_write_cstring(iter, KEY_CCY, p_ccy);

  // Send the message!
  app_message_outbox_send();
}

/*void worker_handler(uint16_t idx, AppWorkerMessage* data)
{
  if (idx == WORKER_DATA)
	{
		Addr addr;
		addr.parts[0] = data->data0;
		addr.parts[1] = data->data1;
		p_bufferIndex = addr.ptr;
		p_buffer = addr.ptr + 1;
	}
  APP_LOG(APP_LOG_LEVEL_INFO, "worker_handler: bufferIndex = %p, buffer = %p", p_bufferIndex, p_buffer);
}*/


void Index_construct(TextLayer* indexLayer, TextLayer* indexLayer2, Layer* graphLayer)
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Location_construct()");
  // Register callbacks
  app_message_register_inbox_received(Index_inboxReceived);
  app_message_register_inbox_dropped(Index_inboxDropped);
  app_message_register_outbox_failed(Index_outboxFailed);
  app_message_register_outbox_sent(Index_outboxSent);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
/*
	app_worker_message_subscribe(worker_handler);

	if (!app_worker_is_running())
	{
		AppWorkerResult res = app_worker_launch();
    APP_LOG(APP_LOG_LEVEL_INFO, "launching worker %d", res);
	}
	// wait for worker to be ready
	while (!app_worker_is_running())
	{ }
*/
  p_ccy = "GBP";
  p_indexDisplay = indexLayer;
  p_indexDisplay2 = indexLayer2;
  p_graphDisplay = graphLayer;
  layer_set_update_proc(p_graphDisplay, drawGraph);

  //Index_handler();
}


void Index_destroy()
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Index_destroy()");
	app_worker_message_unsubscribe();
  //app_message_deregister_callbacks();
}

TimerWatcher Index_getWatcher()
{
  return (TimerWatcher) { .handler = Index_handler, 
                          .updateInterval = 60 / g_updateFreq, 
                          .updateUnits = MINUTE_UNIT };
}

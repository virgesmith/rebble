

#include "Connection.h"
#include "TextBox.h"

static BitmapLayer* p_display;
GBitmap *p_connBitmap;
GBitmap *p_noConnBitmap;

static bool connected = true; // so as no false +ve on init

void Connection_handler()
{
  bool wasConnected = connected;
  connected = bluetooth_connection_service_peek();
  if (wasConnected && ! connected)
  {
    vibes_short_pulse();
  }
//  GColor colour = connected ? GColorClear : GColorRed;
//  bitmap_layer_set_background_color(display, colour);
  bitmap_layer_set_bitmap(p_display, connected ? p_connBitmap : p_noConnBitmap);

}

void Connection_construct(BitmapLayer* layer, int iconIds[2])
{
  p_display = layer;
  //TextBox_write(display, "Bt");
  p_connBitmap = gbitmap_create_with_resource(iconIds[0]);
  p_noConnBitmap = gbitmap_create_with_resource(iconIds[1]);
  //bitmap_layer_set_compositing_mode(layer, GCompOpSet);
  //bitmap_layer_set_bitmap(layer, connBitmap);

  // call handler immediately
  Connection_handler();
}

void Connection_destroy()
{
  gbitmap_destroy(p_connBitmap);
  gbitmap_destroy(p_noConnBitmap);
  p_display = NULL;
}

TimerWatcher Connection_getWatcher()
{
  return (TimerWatcher) { .handler = Connection_handler, 
                                   .updateInterval = 1, 
                                   .updateUnits = MINUTE_UNIT };
}




#include "Click.h"

static ClickHandlers p_clickCallbacks;

// register the callbacks
void Click_init(void* context) 
{
  // single click / repeat-on-hold config:
  if (p_clickCallbacks.up)
  {
    window_single_click_subscribe(BUTTON_ID_UP, p_clickCallbacks.up);  
  }
  if (p_clickCallbacks.down)
  {
    window_single_click_subscribe(BUTTON_ID_DOWN, p_clickCallbacks.down);  
  }
  if (p_clickCallbacks.select)
  {
    window_single_click_subscribe(BUTTON_ID_SELECT, p_clickCallbacks.select);  
  }
  if (p_clickCallbacks.back)
  {
    window_single_click_subscribe(BUTTON_ID_BACK, p_clickCallbacks.back);  
  }
  if (p_clickCallbacks.selectMulti)
  {
    window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 10, 0, true, p_clickCallbacks.selectMulti);
  }
  if (p_clickCallbacks.selectLong || p_clickCallbacks.selectRelease)
  {
    window_long_click_subscribe(BUTTON_ID_SELECT, 700, p_clickCallbacks.selectLong, p_clickCallbacks.selectRelease);
  }
}

// 
void Click_construct(Window* window, ClickHandlers callbacks)
{  
  p_clickCallbacks = callbacks;
  window_set_click_config_provider(window, (ClickConfigProvider)Click_init);
}

void Click_destroy()
{
  //TODO is there anything to do?
}



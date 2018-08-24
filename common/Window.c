
#include "Window.h"

Window* Window_construct(WindowHandlers handlers)
{
  //APP_LOG(APP_LOG_LEVEL_INFO, __FUNCTION__);
  // Create main Window element and assign to pointer
  Window* window = window_create();
  
  //    
  //Click_construct(window);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(window, handlers);
  //APP_LOG(APP_LOG_LEVEL_INFO, "Window handler load: %p", (void*)handlers.load);
  //APP_LOG(APP_LOG_LEVEL_INFO, "Window handler unload: %p", (void*)handlers.unload);
   
  // Show the Window on the watch, with animated=true
  window_stack_push(window, true);
  
  return window;
}

void Window_setBGColour(Window* window, GColor colour)
{
  window_set_background_color(window, colour); 	
}

void Window_destroy(Window* window)
{
  window_destroy(window);
}


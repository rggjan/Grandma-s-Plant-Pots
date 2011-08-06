#include "player.h"

#include <ClanLib/display.h>

Player::Player(CL_DisplayWindow* window) {
  display_window = window;

  gc = &(display_window->get_gc());
  window_width = gc->get_width();
  window_height = gc->get_height();

  cross_x = window_width/2;
  cross_y = window_height/2;
  center_x = 300;
  center_y = 300;
  cross_speed = 0.7;
}

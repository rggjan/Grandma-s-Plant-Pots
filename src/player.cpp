#include "player.h"

#include <iostream>
#include <ClanLib/display.h>

Player::Player(CL_DisplayWindow* window, int width, int height)
  : moving_down(false),
    moving_up(false),
    moving_left(false),
    moving_right(false),
    map_width(width),
    map_height(height) {
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

void Player::update(int timeElapsed_ms) {
  const int min_border = 100;
  const int min_x = min_border;
  const int min_y = min_border;
  const int max_x = window_width - min_border - 1;
  const int max_y = window_height - min_border - 1;

  const int max_center_x = map_width - window_width - 1;
  const int max_center_y = map_height - window_height - 1;

  int move_x = 0;
  int move_y = 0;

  // Move camera
  if (moving_down)
    move_y += timeElapsed_ms*cross_speed;
  if (moving_up)
    move_y -= timeElapsed_ms*cross_speed;
  if (moving_left)
    move_x -= timeElapsed_ms*cross_speed;
  if (moving_right)
    move_x += timeElapsed_ms*cross_speed;

  // Cross Up
  if (move_y < 0) {
    if (cross_y > min_y) {
      cross_y += move_y;
      move_y = 0;
      if (cross_y < min_y) {
        move_y = cross_y - min_y;
        cross_y = min_y;
      }
    }
    if (move_y < 0 && center_y > 0) {
      center_y += move_y;
      move_y = 0;
      if (center_y < 0) {
        move_y = center_y - 0;
        center_y = 0;
      }
    }
    if (move_y < 0) {
      cross_y += move_y;
      if (cross_y < 0)
        cross_y = 0;
    }
  }

  // Cross Left
  if (move_x < 0) {
    if (cross_x > min_x) {
      cross_x += move_x;
      move_x = 0;
      if (cross_x < min_x) {
        move_x = cross_x - min_x;
        cross_x = min_x;
      }
    }
    if (move_x < 0 && center_x > 0) {
      center_x += move_x;
      move_x = 0;
      if (center_x < 0) {
        move_x = center_x - 0;
        center_x = 0;
      }
    }
    if (move_x < 0) {
      cross_x += move_x;
      if (cross_x < 0)
        cross_x = 0;
    }
  }

  // Cross Down
  if (move_y > 0) {
    if (cross_y < max_y) {
      cross_y += move_y;
      move_y = 0;
      if (cross_y > max_y) {
        move_y = cross_y - max_y;
        cross_y = max_y;
      }
    }
    if (move_y > 0 && center_y < max_center_y) {
      center_y += move_y;
      move_y = 0;
      if (center_y > max_center_y) {
        move_y = center_y - max_center_y;
        center_y = max_center_y;
      }
    }
    if (move_y > 0) {
      cross_y += move_y;
      if (cross_y > window_height - 1)
        cross_y = window_height -1;
    }
  }
  // Cross Right
  if (move_x > 0) {
    if (cross_x < max_x) {
      cross_x += move_x;
      move_x = 0;
      if (cross_x > max_x) {
        move_x = cross_x - max_x;
        cross_x = max_x;
      }
    }
    if (move_x > 0 && center_x < max_center_x) {
      center_x += move_x;
      move_x = 0;
      if (center_x > max_center_x) {
        move_x = center_x - max_center_x;
        center_x = max_center_x;
      }
    }
    if (move_x > 0) {
      cross_x += move_x;
      if (cross_x > window_width - 1)
        cross_x = window_width -1;
    }
  }
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./player.h"

#include <ClanLib/core.h>

#define CROSS_SPEED 0.7
#define MIN_BORDER 100

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

  relative_cross_x = window_width/2;
  cross_y = window_height/2;
  center_x = 300;
  center_y = 300;


  CL_FontDescription desc;
  desc.set_height(20);
  default_font = CL_Font_System(*gc, desc);
}

void Player::update(int timeElapsed_ms) {
  const int min_x = MIN_BORDER;
  const int min_y = MIN_BORDER;
  const int max_x = window_width - MIN_BORDER - 1;
  const int max_y = window_height - MIN_BORDER - 1;

  const int max_center_x = map_width - window_width - 1;
  const int max_center_y = map_height - window_height - 1;

  int move_x = 0;
  int move_y = 0;

  // Move camera
  if (moving_down)
    move_y += timeElapsed_ms*CROSS_SPEED;
  if (moving_up)
    move_y -= timeElapsed_ms*CROSS_SPEED;
  if (moving_left)
    move_x -= timeElapsed_ms*CROSS_SPEED;
  if (moving_right)
    move_x += timeElapsed_ms*CROSS_SPEED;

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
    if (relative_cross_x > min_x) {
      relative_cross_x += move_x;
      move_x = 0;
      if (relative_cross_x < min_x) {
        move_x = relative_cross_x - min_x;
        relative_cross_x = min_x;
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
      relative_cross_x += move_x;
      if (relative_cross_x < 0)
        relative_cross_x = 0;
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
    if (relative_cross_x < max_x) {
      relative_cross_x += move_x;
      move_x = 0;
      if (relative_cross_x > max_x) {
        move_x = relative_cross_x - max_x;
        relative_cross_x = max_x;
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
      relative_cross_x += move_x;
      if (relative_cross_x > window_width - 1)
        relative_cross_x = window_width -1;
    }
  }
}

void Player::draw() {
  CL_String s("Life: ");
  s += CL_StringHelp::int_to_text(3);
  default_font.draw_text(*gc, CL_Pointf(10, 30), s, CL_Colorf::white);
}

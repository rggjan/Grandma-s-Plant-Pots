// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./player.h"

#include <ClanLib/core.h>

#include "./world.h"

#define CROSS_SPEED 0.6
#define MIN_BORDER 100

Player::Player(CL_DisplayWindow* window, World* world, int width, int height)
  : moving_down(false),
    moving_up(false),
    moving_left(false),
    moving_right(false),
    map_width(width),
    map_height(height),
    world(world) {
  display_window = window;

  gc = &(display_window->get_gc());
  window_width = gc->get_width();
  window_height = gc->get_height();

  cross_position_ = CL_Vec2f(window_width / 2, window_height / 2);
  map_position_ = CL_Vec2f(300, 300);

  CL_FontDescription desc;
  desc.set_height(20);
  default_font = CL_Font_System(*gc, desc);

  cross = new CL_Sprite(*gc, "Cross", &world->resources);
}

void Player::Update(int timeElapsed_ms) {
  const int min_x = MIN_BORDER;
  const int min_y = MIN_BORDER;
  const int max_x = window_width - MIN_BORDER - 1;
  const int max_y = window_height - MIN_BORDER - 1;

  const int max_map_x = map_width - window_width - 1;
  const int max_map_y = map_height - window_height - 1;

  int move_x = 0;
  int move_y = 0;

  // Move camera
  if (moving_down)
    move_y += timeElapsed_ms * CROSS_SPEED;
  if (moving_up)
    move_y -= timeElapsed_ms * CROSS_SPEED;
  if (moving_left)
    move_x -= timeElapsed_ms * CROSS_SPEED;
  if (moving_right)
    move_x += timeElapsed_ms * CROSS_SPEED;

  // Cross Up
  if (move_y < 0) {
    if (cross_position_.y > min_y) {
      cross_position_.y += move_y;
      move_y = 0;
      if (cross_position_.y < min_y) {
        move_y = cross_position_.y - min_y;
        cross_position_.y = min_y;
      }
    }
    if (move_y < 0 && map_position_.y > 0) {
      map_position_.y += move_y;
      move_y = 0;
      if (map_position_.y < 0) {
        move_y = map_position_.y - 0;
        map_position_.y = 0;
      }
    }
    if (move_y < 0) {
      cross_position_.y += move_y;
      if (cross_position_.y < 0)
        cross_position_.y = 0;
    }
  }

  // Cross Left
  if (move_x < 0) {
    if (cross_position_.x > min_x) {
      cross_position_.x += move_x;
      move_x = 0;
      if (cross_position_.x < min_x) {
        move_x = cross_position_.x - min_x;
        cross_position_.x = min_x;
      }
    }
    if (move_x < 0 && map_position_.x > 0) {
      map_position_.x += move_x;
      move_x = 0;
      if (map_position_.x < 0) {
        move_x = map_position_.x - 0;
        map_position_.x = 0;
      }
    }
    if (move_x < 0) {
      cross_position_.x += move_x;
      if (cross_position_.x < 0)
        cross_position_.x = 0;
    }
  }

  // Cross Down
  if (move_y > 0) {
    if (cross_position_.y < max_y) {
      cross_position_.y += move_y;
      move_y = 0;
      if (cross_position_.y > max_y) {
        move_y = cross_position_.y - max_y;
        cross_position_.y = max_y;
      }
    }
    if (move_y > 0 && map_position_.y < max_map_y) {
      map_position_.y += move_y;
      move_y = 0;
      if (map_position_.y > max_map_y) {
        move_y = map_position_.y - max_map_y;
        map_position_.y = max_map_y;
      }
    }
    if (move_y > 0) {
      cross_position_.y += move_y;
      if (cross_position_.y > window_height - 1)
        cross_position_.y = window_height - 1;
    }
  }
  // Cross Right
  if (move_x > 0) {
    if (cross_position_.x < max_x) {
      cross_position_.x += move_x;
      move_x = 0;
      if (cross_position_.x > max_x) {
        move_x = cross_position_.x - max_x;
        cross_position_.x = max_x;
      }
    }
    if (move_x > 0 && map_position_.x < max_map_x) {
      map_position_.x += move_x;
      move_x = 0;
      if (map_position_.x > max_map_x) {
        move_x = map_position_.x - max_map_x;
        map_position_.x = max_map_x;
      }
    }
    if (move_x > 0) {
      cross_position_.x += move_x;
      if (cross_position_.x > window_width - 1)
        cross_position_.x = window_width - 1;
    }
  }
}

void Player::DrawFloor() {
}

void Player::DrawTop() {
  // Draw cross
  cross->set_scale(0.5, 0.5);
  cross->draw(*gc, cross_position_.x, cross_position_.y);
  cross->set_scale(0.25, 0.25);
  cross->draw(*gc, cross_position_.x, cross_position_.y);
}

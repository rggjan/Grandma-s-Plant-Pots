// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./player.h"

#include "./world.h"

#define CROSS_SPEED 0.6
#define MIN_BORDER 100

Player::Player(CL_GraphicContext* gc, World* world, int width, int height)
  : gc_(gc),
    map_position_(300, 300),
    cross_position_(window_width_ / 2, window_height_ / 2),
    moving_down_(false),
    moving_up_(false),
    moving_left_(false),
    moving_right_(false),
    window_width_(gc_->get_width()),
    window_height_(gc_->get_height()),
    map_width_(width),
    map_height_(height),
    cross_(*gc, "Cross", &world->resources),
    world_(world) {
  CL_FontDescription desc;
  desc.set_height(20);
  default_font_ = CL_Font_System(*gc_, desc);
}

bool Player::Visible(CL_Vec2f position) {
  CL_Vec2f pos = position - map_position_;

  if (pos.x < 0)
    return false;
  if (pos.y < 0)
    return false;
  if (pos.x > window_width_)
    return false;
  if (pos.y > window_height_)
    return false;

  return true;
}

void Player::Update(int timeElapsed_ms) {
  const int min_x = MIN_BORDER;
  const int min_y = MIN_BORDER;
  const int max_x = window_width_ - MIN_BORDER - 1;
  const int max_y = window_height_ - MIN_BORDER - 1;

  const int max_map_x = map_width_ - window_width_ - 1;
  const int max_map_y = map_height_ - window_height_ - 1;

  int move_x = 0;
  int move_y = 0;

  // Move camera
  if (moving_down_)
    move_y += timeElapsed_ms * CROSS_SPEED;
  if (moving_up_)
    move_y -= timeElapsed_ms * CROSS_SPEED;
  if (moving_left_)
    move_x -= timeElapsed_ms * CROSS_SPEED;
  if (moving_right_)
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
      if (cross_position_.y > window_height_ - 1)
        cross_position_.y = window_height_ - 1;
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
      if (cross_position_.x > window_width_ - 1)
        cross_position_.x = window_width_ - 1;
    }
  }
}

void Player::DrawFloor() {
}

void Player::DrawTop() {
  // Draw cross_
  cross_.set_scale(0.5, 0.5);
  cross_.draw(*gc_, cross_position_.x, cross_position_.y);
  cross_.set_scale(0.25, 0.25);
  cross_.draw(*gc_, cross_position_.x, cross_position_.y);
}

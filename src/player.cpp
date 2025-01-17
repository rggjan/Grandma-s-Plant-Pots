// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./player.h"

#include "./world.h"

#define CROSS_SPEED 0.6f
#define MIN_BORDER 100

Player::Player(clan::Canvas canvas, World* world, clan::Sprite map, clan::Size window_size)
  : canvas_(canvas),
    map_position_(300, 300),
    cross_position_(window_size.width / 2.f, window_size.height / 2.f),
    moving_down_(false),
    moving_up_(false),
    moving_left_(false),
    moving_right_(false),
    world_(world),
    window_size_(window_size),
    map_(map),
    cross_(clan::Sprite::resource(canvas, "Cross", world->resources)){
  clan::FontDescription desc;
  desc.set_height(20);
  desc.set_typeface_name("Tahoma");
  default_font_ = clan::Font(canvas_, desc);

  desc.set_height(15);
  small_font_= clan::Font(canvas_,desc);
}

bool Player::Visible(clan::Vec2f position) {
  clan::Vec2f pos = position - map_position_;

  if (pos.x < 0)
    return false;
  if (pos.y < 0)
    return false;
  if (pos.x > window_size_.width)
    return false;
  if (pos.y > window_size_.height)
    return false;

  return true;
}

void Player::Update(int timeElapsed_ms) {
  const float min_x = MIN_BORDER;
  const float min_y = MIN_BORDER;
  const float max_x = window_size_.width - MIN_BORDER - 1.f;
  const float max_y = window_size_.height - MIN_BORDER - 1.f;

  const float max_map_x = map_.get_width() - window_size_.width - 1.f;
  const float max_map_y = map_.get_height() - window_size_.height - 1.f;

  float move_x = 0;
  float move_y = 0;

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
      if (cross_position_.y > window_size_.height - 1)
        cross_position_.y = window_size_.height - 1;
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
      if (cross_position_.x > window_size_.width - 1)
        cross_position_.x = window_size_.width - 1;
    }
  }
}

void Player::DrawFloor() {
  map_.draw(canvas_, -map_position_.x, -map_position_.y);
}

void Player::DrawTop() {
  // Draw cross
  cross_.set_scale(0.5, 0.5);
  cross_.draw(canvas_, cross_position_.x, cross_position_.y);
  cross_.set_scale(0.25, 0.25);
  cross_.draw(canvas_, cross_position_.x, cross_position_.y);
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

class World;

class Player {
  public:
    Player(clan::Canvas canvas, World* world, clan::Sprite map, clan::Size window_size);
    virtual ~Player() {}

    virtual void BuildButtonPressed() {}
    virtual void SelectButtonPressed() {}
    virtual void CancelButtonPressed() {}

    virtual void MovingUpButtonPressed() { moving_up_ = true; }
    virtual void MovingDownButtonPressed() { moving_down_ = true; }
    virtual void MovingLeftButtonPressed() { moving_left_ = true; }
    virtual void MovingRightButtonPressed() { moving_right_ = true; }

    virtual void MovingUpButtonReleased() { moving_up_ = false; }
    virtual void MovingDownButtonReleased() { moving_down_ = false; }
    virtual void MovingLeftButtonReleased() { moving_left_ = false; }
    virtual void MovingRightButtonReleased() { moving_right_ = false; }

    virtual void Update(int time_ms);
    virtual void DrawTop();
    virtual void DrawFloor();

    inline clan::Vec2f position() { return map_position_ + cross_position_; }
    inline clan::Vec2f cross_position() { return cross_position_; }
    inline clan::Vec2f map_position() { return map_position_; }

    bool Visible(clan::Vec2f position);

    clan::Canvas canvas_;

    clan::Vec2f map_position_;
    clan::Vec2f cross_position_;

    bool moving_down_;
    bool moving_up_;
    bool moving_left_;
    bool moving_right_;

  protected:
    World* world_;

    clan::Size window_size_;

    clan::Sprite map_;
    clan::Font default_font_;
    clan::Font small_font_;
    clan::Sprite cross_;
};

#endif  // SRC_PLAYER_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <ClanLib/display.h>

class World;

class Player {
  public:
    Player(CL_GraphicContext* gc, World* world, int width, int height);
    virtual ~Player() {}

    virtual void BuildButtonPressed() {}
    virtual void SelectButtonPressed() {}
    virtual void CancelButtonPressed() {}

    virtual void Update(int time_ms);
    virtual void DrawTop();
    virtual void DrawFloor();

    inline CL_Vec2f position() { return map_position_ + cross_position_; }
    inline CL_Vec2f cross_position() { return cross_position_; }
    inline CL_Vec2f map_position() { return map_position_; }

    CL_GraphicContext *gc_;

    CL_Vec2f map_position_;
    CL_Vec2f cross_position_;

    bool moving_down;
    bool moving_up;
    bool moving_left;
    bool moving_right;

    bool Visible(CL_Vec2f position);

  protected:
    int window_width, window_height;
    int map_width, map_height;
    CL_Font default_font;
    CL_Sprite *cross;

    World* world;
};

#endif  // SRC_PLAYER_H_

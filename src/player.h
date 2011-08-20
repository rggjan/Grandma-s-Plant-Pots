// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <ClanLib/display.h>

class CL_DisplayWindow;
class CL_GraphicContext;
class CL_Font;
class World;

class Player {
  public:
    Player(CL_DisplayWindow* window, World* world, int width, int height);
    virtual ~Player() {}

    virtual void BuildButtonPressed() {}

    virtual void update(int timeElapsed_ms);
    virtual void draw();

    CL_GraphicContext *gc;
    CL_DisplayWindow *display_window;

    int center_x;
    int center_y;

    bool moving_down;
    bool moving_up;
    bool moving_left;
    bool moving_right;

    int cross_x, cross_y;

  protected:
    int window_width, window_height;
    int map_width, map_height;
    CL_Font default_font;

    int energy;

    World* world;
};

#endif  // SRC_PLAYER_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include <ClanLib/display.h>

class CL_DisplayWindow;
class CL_GraphicContext;
class CL_Font;

class Player {
  public:
    Player(CL_DisplayWindow* window, int width, int height);

    void update(int timeElapsed_ms);

    void draw();

    CL_GraphicContext *gc;
    CL_DisplayWindow *display_window;

    int center_x;
    int center_y;

    bool moving_down;
    bool moving_up;
    bool moving_left;
    bool moving_right;

    int relative_cross_x;
    int cross_y;

  private:
    int window_width, window_height;
    int map_width, map_height;
    CL_Font default_font;
};

#endif  // SRC_PLAYER_H_

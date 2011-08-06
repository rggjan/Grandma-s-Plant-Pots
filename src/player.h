// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

class CL_DisplayWindow;
class CL_GraphicContext;

class Player {
  public:
    Player(CL_DisplayWindow* window);
    CL_GraphicContext *gc;
    CL_DisplayWindow *display_window;

    int center_x;
    int center_y;
    
  private:
    int window_width;
    int window_height;

    int cross_x;
    int cross_y;

    float cross_speed;
};

#endif  // SRC_PLAYER_H_

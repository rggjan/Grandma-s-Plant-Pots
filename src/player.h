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
    virtual void SelectButtonPressed() {}
    virtual void CancelButtonPressed() {}

    virtual void update(int timeElapsed_ms);
    virtual void DrawEnergy();
    virtual void draw();

    inline int x() {
      return center_x + cross_x;
    }
    inline int y() {
      return center_y + cross_y;
    }

    CL_GraphicContext *gc;
    CL_DisplayWindow *display_window;

    int center_x, center_y;
    int cross_x, cross_y;

    bool moving_down;
    bool moving_up;
    bool moving_left;
    bool moving_right;


  protected:
    int window_width, window_height;
    int map_width, map_height;
    CL_Font default_font;
    CL_Sprite *cross;

    int energy;

    World* world;

    virtual void draw_cross();
};

#endif  // SRC_PLAYER_H_

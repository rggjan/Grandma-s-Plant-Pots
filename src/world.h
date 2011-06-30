// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <ClanLib/display.h>
#include <list>

class CL_Surface;
class CL_InputEvent;
class GameObject;
class TankVehicle;
class Background;

class World {
  // Construction
  public:
    World(CL_DisplayWindow &display_window);
    ~World();

  // Attributes:
  public:
    CL_ResourceManager resources;

    CL_GraphicContext get_gc() {
      return gc;
    }

    int center_x;
    int center_y;

    bool moving_down;
    bool moving_up;
    bool moving_left;
    bool moving_right;

// Operations:
  public:
    void initLevel();

    bool hitCheck(CL_CollisionOutline *outline, GameObject *other);

    void addObject(GameObject *object);
    void addTank(TankVehicle *tank);

    void run();

  // Implementation:
  private:
    void draw();
    void update();

    int calcTimeElapsed();

    void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
    void onKeyUp(const CL_InputEvent &key, const CL_InputState &state);
    void onMouseDown(const CL_InputEvent &key, const CL_InputState &state);
    void onMouseUp(const CL_InputEvent &key, const CL_InputState &state);
    void onMouseMove(const CL_InputEvent &key, const CL_InputState &state);
    void on_window_close();

    CL_Slot slotMouseDown;
    CL_Slot slotMouseUp;
    CL_Slot slotMouseMove;
    CL_Slot slotKeyDown;
    CL_Slot slotKeyUp;

    Background *background;

    bool dragging;
    bool mouseDown;
    CL_Rect dragArea;
    float highlightValue;

    std::list<GameObject *> objects;
    std::list<TankVehicle *> tanks;

    CL_DisplayWindow window;
    CL_GraphicContext gc;

    bool quit;
};

#endif  // SRC_WORLD_H

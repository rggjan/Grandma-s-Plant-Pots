// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <ClanLib/display.h>
#include <list>
#include <vector>

#define MAX_PLAYERS 8

class CL_Surface;
class CL_InputEvent;
class GameObject;
class Background;
class Fly;
class Player;
class Flower;
class Leaf;

class World {
    // Construction
  public:
    explicit World(std::vector<CL_DisplayWindow*> windows);
    ~World();

    // Attributes:
  public:
    CL_ResourceManager resources;

    bool quit;

    int num_players;

    int width;
    int height;

    // Operations:
  public:
    void initLevel();

    bool hitCheck(CL_CollisionOutline *outline, GameObject *other);

    void addObject(GameObject *object);
    void addFly(Fly *fly);
    void addFlower(Flower *flower);
    void addLeaf(Leaf *leaf);

    void run();

    bool CanBuild(int x, int y);

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

    // TODO(rggjan): set as max players
    CL_Slot slotKeyDown[MAX_PLAYERS];
    CL_Slot slotKeyUp[MAX_PLAYERS];
    CL_Slot slotQuit[MAX_PLAYERS];

    CL_Sprite *background;

    bool dragging;
    bool mouseDown;
    CL_Rect dragArea;
    float highlightValue;

    std::vector<GameObject *> objects;
    std::vector<Fly *> flies;
    std::vector<Player*> players;

    std::vector<Flower*> flowers;

    CL_GraphicContext default_gc;
};

#endif  // SRC_WORLD_H_

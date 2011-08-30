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
  public:
    // Construction
    explicit World(std::vector<CL_DisplayWindow*> windows);
    ~World();

    // Attributes
    CL_ResourceManager resources;
    bool quit;
    int num_players;
    int width;
    int height;

    // Operations
    void addObject(GameObject *object);
    void addFly(Fly *fly);
    void addFlower(Flower *flower);
    void run();

    // Queries
    Flower* NearestFlower(CL_Vec2f position);
    std::vector<Flower *>* NearestFlowers(CL_Vec2f position);
    bool CanBuild(CL_Vec2f position);

  private:
    // Attributes
    CL_Slot slotKeyDown[MAX_PLAYERS];
    CL_Slot slotKeyUp[MAX_PLAYERS];
    CL_Slot slotQuit[MAX_PLAYERS];

    CL_Sprite *background;

    std::vector<GameObject *> objects;
    std::vector<Fly *> flies;
    std::vector<Flower*> flowers;
    std::vector<Player*> players;

    CL_GraphicContext default_gc;
    CL_Font default_font_;

    int time_elapsed_ms_;
    float fps_;

    // Operations
    void Draw();
    void Update();
    int CalcTimeElapsed();

    void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
    void onKeyUp(const CL_InputEvent &key, const CL_InputState &state);
    void on_window_close();
};

#endif  // SRC_WORLD_H_

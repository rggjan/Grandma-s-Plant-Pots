// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <ClanLib/display.h>
#include <list>

#define MAX_PLAYERS 8

class Bug;
class Flower;
class Plant;
class Player;
class CL_SoundBuffer;

class World {
  public:
    // Construction
    explicit World(CL_DisplayWindow* window);
    ~World();

    // Attributes
    CL_ResourceManager resources;
    bool quit;
    int num_players;
    int width;
    int height;
    int player_width_;
    int player_height_;
    CL_SoundBuffer *sound_beep1_;

    // Operations
    void AddBug(Bug *bug);
    void RemoveBug(Bug *bug);

    void AddMasterPlant(Plant *plant);
    void RemoveMasterPlant(Plant *plant);

    void AddPlant(Plant *plant);
    void RemovePlant(Plant *plant);

    void Run();
    void PlayBeep();

    // Queries
    Plant* NearestMasterPlant(CL_Vec2f position);
    std::list<Bug *>* NearestBugs(CL_Vec2f position);
    std::list<Plant *>* NearestPlants(CL_Vec2f position);
    bool CanBuild(CL_Vec2f position);

  private:
    // Attributes
    CL_Slot slotKeyDown[MAX_PLAYERS];
    CL_Slot slotKeyUp[MAX_PLAYERS];
    CL_Slot slotQuit[MAX_PLAYERS];

    CL_Sprite *background;

    std::list<Bug*> bugs;
    std::list<Plant*> master_plants;
    std::list<Plant*> plants;

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

    CL_DisplayWindow* window_;

    CL_FrameBuffer* framebuffer_;
    CL_Texture* texture_;
};

#endif  // SRC_WORLD_H_

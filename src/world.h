// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_WORLD_H_
#define SRC_WORLD_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/Core/Resources/resource_manager.h>
#include <ClanLib/Sound/soundbuffer.h>
#include <list>
#include <vector>

#define MAX_PLAYERS 8

class Bug;
class Flower;
class Plant;
class Player;
namespace clan {
  class SoundBuffer;
  class InputState;
}

class World {
 public:
  // Construction
  explicit World(clan::DisplayWindow* window);
  ~World();

  // Attributes
  clan::ResourceManager resources;
  bool quit;
  int num_players;
  int width;
  int height;
  int player_width_;
  int player_height_;
  clan::SoundBuffer sound_beep1_;

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
  Plant* NearestMasterPlant(clan::Vec2f position);
  std::list<Bug *>* NearestBugs(clan::Vec2f position);
  std::list<Plant *>* NearestPlants(clan::Vec2f position);
  bool CanBuild(clan::Vec2f position);

 private:
  // Attributes
  clan::Slot slotKeyDown[MAX_PLAYERS];
  clan::Slot slotKeyUp[MAX_PLAYERS];
  clan::Slot slotQuit[MAX_PLAYERS];

  clan::Sprite background;

  std::list<Bug*> bugs;
  std::list<Plant*> master_plants;
  std::list<Plant*> plants;

  std::vector<Player*> players;

  clan::Canvas canvas_;
  clan::Font default_font_;

  int time_elapsed_ms_;
  double fps_;

  // Operations
  void Draw();
  void Update();
  int CalcTimeElapsed();

  void onKeyDown(const clan::InputEvent &key);
  void onKeyUp(const clan::InputEvent &key);
  void on_window_close();

  clan::DisplayWindow* window_;

  clan::FrameBuffer* framebuffer_;
  clan::Texture* texture_;
};

#endif  // SRC_WORLD_H_

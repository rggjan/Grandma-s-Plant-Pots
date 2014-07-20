// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANTPLAYER_H_
#define SRC_PLANTS_PLANTPLAYER_H_

#include <string>
#include <ClanLib/sound.h>

#include <list>

#include "./player.h"

class Flower;
class Tower;
class Leaf;
class Plant;

enum State {
  Idle,
  BuildMenu,
  Building,
  Selecting,
  Selected,
  SelectedBuilding,
};

class PlantPlayer : public Player {
  public:
    // Constructor
    PlantPlayer(clan::Canvas canvas, World *world, clan::Sprite map, clan::Size window_size);
    ~PlantPlayer();

    void MovingLeftButtonPressed();
    void MovingRightButtonPressed();

    // Operations
    void BuildButtonPressed();
    void SelectButtonPressed();
    void CancelButtonPressed();

    void DrawFloor();
    void DrawTop();

    void Update(int time_ms);

    double co2_;
    double sugar_;
    double sun_;
    double iron_;

    // Can the player build it? (Sugar etc.)
    bool CanBuild(Plant *plant);

  private:
    // Operations
    void DrawEnergy();
    void DrawCO2();
    void DrawSun();
    void DrawIron();

    bool BuildPlant(Plant *plant);

    // Queries
    Plant* NearestPlant();

    // Attributes
    std::list<Plant*> plants_;
    State state_;
    clan::Sprite select_sprite_;
    clan::Sprite co2_sprite_;
    clan::Sprite sun_sprite_;
    clan::Sprite sugar_sprite_;
    clan::Sprite iron_sprite_;
    Plant *selected_plant_;

    std::vector<Plant*> plant_menu_;
    int menu_item_;
    //Leaf *tmp_leaf_;

    clan::SoundBuffer sound_plantgrowing_;
    clan::SoundBuffer sound_leafgrowing_;
};

#endif  // SRC_PLANTS_PLANTPLAYER_H_

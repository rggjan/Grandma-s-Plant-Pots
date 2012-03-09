// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANTPLAYER_H_
#define SRC_PLANTS_PLANTPLAYER_H_

#include <ClanLib/sound.h>

#include <list>

#include "./player.h"

class Flower;
class Tower;
class Leaf;
class CL_SoundBuffer;
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
    PlantPlayer(CL_GraphicContext* gc, World *world, int width, int height);
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

  private:
    // Operations
    bool BuildPlant();
    bool BuildLeaf();
    void DrawEnergy();
    void DrawCO2();
    void DrawSun();

    bool CanBuild(Plant *plant);
    bool BuildPlant(Plant *plant);    

    // Queries
    Flower* NearestFlower();

    // Attributes
    std::list<Plant*> plants_;
    State state;
    CL_Sprite select_sprite_;
    Plant *selected_plant_;

    std::vector<Plant*> plant_menu_;
    int menu_item_;
    //Leaf *tmp_leaf_;

    CL_SoundBuffer sound_plantgrowing_;
    CL_SoundBuffer sound_leafgrowing_;
};

#endif  // SRC_PLANTS_PLANTPLAYER_H_

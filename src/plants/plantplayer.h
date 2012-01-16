// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANTPLAYER_H_
#define SRC_PLANTS_PLANTPLAYER_H_

#include <ClanLib/sound.h>

#include <list>

#include "./player.h"

class Flower;
class Leaf;
class CL_SoundBuffer;

enum State {
  Idle,
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
    bool BuildFlower();
    bool BuildLeaf();
    void DrawEnergy();
    void DrawCO2();
    void DrawSun();

    // Queries
    Flower* NearestFlower();

    // Attributes
    std::list<Flower*> flowers;
    State state;
    CL_Sprite select_sprite_;
    Flower *selected_flower_;
    Flower *tmp_flower_;
    Leaf *tmp_leaf_;
    bool cross_green_;

    CL_SoundBuffer sound_plantgrowing_;
    CL_SoundBuffer sound_leafgrowing_;
};

#endif  // SRC_PLANTS_PLANTPLAYER_H_

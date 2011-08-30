// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_PLANTPLAYER_H_
#define SRC_PLANTS_PLANTPLAYER_H_

#include <vector>

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
    PlantPlayer(CL_DisplayWindow* window, World *world, int width, int height);

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
    std::vector<Flower*> flowers;
    State state;
    CL_Sprite *selectedImage;
    Flower *selectedFlower;    
    Flower *tmpFlower;
    Leaf *tmpLeaf;
    bool cross_green_;

    CL_SoundBuffer *sound_plantgrowing_;
    CL_SoundBuffer *sound_beep1_;
};

#endif  // SRC_PLANTS_PLANTPLAYER_H_

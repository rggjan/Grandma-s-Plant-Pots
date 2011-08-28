// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTPLAYER_H_
#define SRC_PLANTPLAYER_H_

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
    void DrawEnergy();
    void Draw();
    void Update(int time_ms);

    inline float energy() { return energy_; }
    inline void increase_energy(float amount) { energy_ += amount; };

private:
    // Operations
    bool BuildPlant();
    bool BuildLeaf();

    // Queries
    Flower* NearestFlower();

    // Variables
    std::vector<Flower*> flowers;
    State state;
    CL_Sprite *selectedImage;
    Flower *tmpFlower;
    Leaf *tmpLeaf;
    Flower *selectedFlower;
    CL_SoundBuffer *sound_plantgrowing_;
    CL_SoundBuffer *sound_beep1_;
    bool cross_green_;
    float energy_;

  protected:
    virtual void draw_cross();
};

#endif  // SRC_PLANTPLAYER_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTPLAYER_H_
#define SRC_PLANTPLAYER_H_

#include <vector>

#include "./player.h"

class Flower;

enum State {
  Idle,
  Building
};

class PlantPlayer : public Player {
  public:
    PlantPlayer(CL_DisplayWindow* window, World *world, int width, int height);

    void BuildButtonPressed();
    void SelectButtonPressed();
    void CancelButtonPressed();
    void DrawEnergy();
    void draw();

  private:
    std::vector<Flower*> flowers;
    State state;

    CL_Sprite *selectedImage;
    Flower *tmpFlower;

    bool BuildPlant();


  protected:
    virtual void draw_cross();
};

#endif  // SRC_PLANTPLAYER_H_

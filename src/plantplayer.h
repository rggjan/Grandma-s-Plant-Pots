// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTPLAYER_H_
#define SRC_PLANTPLAYER_H_

#include <vector>

#include "./player.h"

class Flower;

class PlantPlayer : public Player {
  public:
    PlantPlayer(CL_DisplayWindow* window, World *world, int width, int height);

    void BuildButtonPressed();
    void draw();

  private:
    std::vector<Flower*> flowers;

    CL_Sprite *selectedImage;
};

#endif  // SRC_PLANTPLAYER_H_

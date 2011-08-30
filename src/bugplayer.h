// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGPLAYER_H_
#define SRC_BUGPLAYER_H_

#include <vector>

#include "./player.h"
#include "./fly.h"

class BugPlayer : public Player {
  public:
    BugPlayer(CL_DisplayWindow* window, World *world, int width, int height);

    void BuildButtonPressed();
    void SelectButtonPressed();
    void CancelButtonPressed();

    void AddFly(Fly* fly);
    void Update(int time_ms);
    void DrawFloor();

  private:

    Flower* GetFreeFlower();
    Fly* GetFreeBug();

    std::vector<Fly*> flies;

    CL_Sprite *selectedImage;

    Flower* nearest_free_flower_;
};

#endif  // SRC_BUGPLAYER_H_

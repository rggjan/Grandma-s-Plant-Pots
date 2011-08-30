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
    // Attributes
    std::vector<Fly*> flies;
    CL_Sprite *selectedImage;
    Plant* nearest_free_plant_;

    // Queries
    Plant* GetFreePlant();
    Fly* GetFreeBug();
};

#endif  // SRC_BUGPLAYER_H_

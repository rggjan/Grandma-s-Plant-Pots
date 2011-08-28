// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUG_H_
#define SRC_BUG_H_

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
    void Draw();

  private:
    std::vector<Fly*> flies;

    CL_Sprite *selectedImage;
};

#endif  // SRC_BUG_H_
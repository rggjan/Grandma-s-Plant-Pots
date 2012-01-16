// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGS_BUGPLAYER_H_
#define SRC_BUGS_BUGPLAYER_H_

#include <ClanLib/sound.h>
#include <list>

#include "./player.h"
#include "bugs/bug.h"


class CL_SoundBuffer;

class BugPlayer : public Player {
  public:
    BugPlayer(CL_GraphicContext* gc, World *world, int width, int height);

    void BuildButtonPressed();
    void SelectButtonPressed();
    void CancelButtonPressed();

    void AddBug(Bug* bug);
    void CreateBug(CL_StringRef name, CL_Vec2f position);
    void Update(int time_ms);
    void DrawTop();
    void DrawFloor();

  private:
    // Attributes
    std::list<Bug*> bugs;
    CL_Sprite select_sprite_;
    Plant* nearest_free_plant_;

    // Queries
    Plant* GetFreePlant();
    Bug* GetFreeBug();
    CL_SoundBuffer sound_bug_attack_;
};

#endif  // SRC_BUGS_BUGPLAYER_H_

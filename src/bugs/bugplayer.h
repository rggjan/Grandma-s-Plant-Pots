// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGS_BUGPLAYER_H_
#define SRC_BUGS_BUGPLAYER_H_

#include <string>
#include <ClanLib/sound.h>
#include <list>

#include "./player.h"
#include "bugs/bug.h"

class BugPlayer : public Player {
  public:
    BugPlayer(clan::Canvas canvas, World *world, clan::Sprite map, clan::Size window_size);

    void BuildButtonPressed();
    void SelectButtonPressed();
    void CancelButtonPressed();

    void AddBug(Bug* bug);
    void CreateBug(std::string name, clan::Vec2f position);
    void Update(int time_ms);
    void DrawTop();
    void DrawFloor();

  private:
    // Attributes
    std::list<Bug*> bugs;
    clan::Sprite select_sprite_;
    Plant* nearest_free_plant_;

    // Queries
    Plant* GetFreePlant();
    Bug* GetFreeBug();
    clan::SoundBuffer sound_bug_attack_;
};

#endif  // SRC_BUGS_BUGPLAYER_H_

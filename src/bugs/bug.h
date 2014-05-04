// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGS_BUG_H_
#define SRC_BUGS_BUG_H_

#include "./gameobject.h"

class Plant;
class BugPlayer;

class Bug : public GameObject {
  public:
    // Construction
    Bug(World *world, clan::Canvas canvas, clan::Vec2f position,
        const std::string name, BugPlayer* player);
    ~Bug();

    // Operations:
    inline void set_position(clan::Vec2f position) {
      position_ = position;
    }

    void SetTargetPlant(Plant *flower);

    virtual bool Update(int time_ms, clan::Vec2f target_position);

    inline bool is_free() {
      return target_plant_ == NULL;
    }

    virtual double DecreaseEnergy(double amount);
    void StopEating();

    // Implementation:
  private:
    clan::Vec2f direction;
    clan::Vec2f target_position_;
    Plant *target_plant_;

    double food_eaten_;
    std::string bug_name_;
    BugPlayer *player_;

    float curve_;
};

#endif  // SRC_BUGS_BUG_H_

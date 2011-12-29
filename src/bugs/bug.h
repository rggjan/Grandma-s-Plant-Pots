// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGS_BUG_H_
#define SRC_BUGS_BUG_H_

#include "./gameobject.h"

class Plant;
class BugPlayer;

class Bug : public GameObject {
  public:
    // Construction
    Bug(World *world, CL_GraphicContext *gc, CL_Vec2f position,
        const CL_StringRef name, BugPlayer* player);
    ~Bug();

    // Operations:
    inline void set_position(CL_Vec2f position) {
      position_ = position;
    }

    void SetTargetPlant(Plant *flower);

    virtual bool Update(int time_ms, CL_Vec2f target_position);

    inline bool is_free() {
      return target_plant_ == NULL;
    }

    virtual double DecreaseEnergy(double amount);
    void StopEating();

    // Implementation:
  private:
    CL_Vec2f direction;
    CL_Vec2f target_position_;
    Plant *target_plant_;

    double food_eaten_;
    CL_StringRef bug_name_;
    BugPlayer *player_;

    double curve_;
};

#endif  // SRC_BUGS_BUG_H_

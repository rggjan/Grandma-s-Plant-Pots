// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_BUGS_FLY_H_
#define SRC_BUGS_FLY_H_

#include "./gameobject.h"

class Plant;
class BugPlayer;

class Fly : public GameObject {
  public:
    // Construction
    Fly(World *world, CL_GraphicContext &gc, const CL_StringRef &name,
        BugPlayer* player);

    // Operations:
    inline void set_position(CL_Vec2f position) {
      position_ = position;
    }
    inline void set_target_position(CL_Vec2f target_position) {
      target_position_ = target_position;
    }
    void SetTargetPlant(Plant *flower);
    virtual bool update(int timeElapsed_ms);
    inline bool is_free() {
      return target_plant_ == NULL;
    }
    void StopEating();

    // Implementation:
  private:
    CL_Vec2f direction;
    CL_Vec2f target_position_;
    Plant *target_plant_;
    
    double energy_;
    
    double food_eaten_;
    CL_StringRef fly_name_;
    BugPlayer *player_;
  };

#endif  // SRC_BUGS_FLY_H_

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_FLOWER_H_
#define SRC_PLANTS_FLOWER_H_

#include <string>

#include <ClanLib/sound.h>

#include <list>

#include "./masterplant.h"
#include "./leaf.h"

class Flower : public MasterPlant {
  public:
    // Static
    virtual inline int sugar_cost() { return 4; }

    // Construction
    Flower(World *world, clan::Canvas canvas,
           clan::Vec2f position, PlantPlayer* player, bool menu = false);

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas canvas);
    void DrawTmpChild(clan::Canvas canvas);
    virtual Plant *get_tmp_child() { return menu_leaf_; }

    ~Flower();

    // Operations
    void AddLeaf(Leaf* leaf);
    bool Update(int time_ms);
    void Draw(clan::Canvas canvas, clan::Vec2f target);
    virtual bool has_children() { return true; }    

    // Queries
    Leaf* NearestLeaf(clan::Vec2f position);

    bool open() { return open_; }

  private:
    std::list<Leaf*> leaves;
    int age_;
    bool open_;

    Leaf *menu_leaf_;
};

#endif  // SRC_PLANTS_FLOWER_H_

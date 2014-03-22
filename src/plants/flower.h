// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PLANTS_FLOWER_H_
#define SRC_PLANTS_FLOWER_H_

#include <ClanLib/sound.h>

#include <list>

#include "./plant.h"

#define ENERGY_COST 6

class Leaf;

class Flower : public Plant {
  public:
    // Static
    virtual inline int sugar_cost() { return ENERGY_COST; }

    // Construction
    Flower(World *world, clan::Canvas *canvas,
           clan::Vec2f position, PlantPlayer* player, bool menu = false);

    virtual Plant *GetNewPlant(clan::Vec2f position, clan::Canvas *canvas);
    void DrawTmpChild(clan::Canvas *canvas);

    ~Flower();

    // Operations
    void AddLeaf(Leaf* leaf);
    bool Update(int time_ms);
    void Draw(clan::Canvas* canvas, clan::Vec2f target);
    virtual bool has_children() { return true; }    

    // Queries
    bool CanBuild(clan::Vec2f position);
    Leaf* NearestLeaf(clan::Vec2f position);

    bool open() { return open_; }

  private:
    std::list<Leaf*> leaves;
    int age_;
    bool open_;

    Leaf *menu_leaf_;    
};

#endif  // SRC_PLANTS_FLOWER_H_

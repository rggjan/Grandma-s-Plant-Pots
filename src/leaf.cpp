// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./leaf.h"

#include <stdlib.h>
#include <ClanLib/display.h>

#include "./world.h"

Leaf::Leaf(World *world, CL_GraphicContext *gc, const CL_StringRef &name,
           CL_Vec2f position, Flower* flower)
  : GameObject(world, position),
    flower_(flower) {
  spriteImage = new CL_Sprite(*gc, name, &world->resources);
}

void Leaf::DrawRed(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::red);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, position.x, position.y);
}

void Leaf::DrawGreen(CL_GraphicContext *gc, CL_Vec2f position) {
  // Draw graphic
  spriteImage->set_color(CL_Color::green);
  spriteImage->set_alpha(0.8);
  spriteImage->set_frame(spriteImage->get_frame_count()-1);
  spriteImage->draw(*gc, position.x, position.y);
}

void Leaf::SetAngle(CL_Angle angle) {
  spriteImage->set_angle(angle);
}

bool Leaf::CanBuild(CL_Vec2f position, Flower* flower) {
  //Flower *nearest_flower = world->NearestFlower(position);

  /*if (nearest_flower &&
      (nearest_flower->position() - position).length() < MIN_FLOWER_DISTANCE)
    return false;*/
  //return world->CanBuild(position);
  return true;
}

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include <vector>

#include "./world.h"

int Application::main(const std::vector<CL_String> &args) {
  // Create a window
  CL_DisplayWindowDescription desc;
  desc.set_title("Grandma's Plant Pots");
  // Use this resolution (as caption is disabled)
  desc.set_size(CL_Size(1024, 768), true);

  CL_DisplayWindow window(desc);

  CL_GraphicContext gc = window.get_gc();

  CL_SoundOutput output(44100);

  // Create world
  World world(window);

  // Run the main loop
  world.run();

  return 0;
}

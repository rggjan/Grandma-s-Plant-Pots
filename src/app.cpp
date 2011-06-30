#include "precomp.h"
#include "app.h"
#include "world.h"

int Application::main(const std::vector<CL_String> &args) {
  // Create a window
  CL_DisplayWindowDescription desc;
  desc.set_title("Grandma's Plant Pots");
  desc.set_size(CL_Size(1024, 768), true);	// Use this resolution (as caption is disabled)
  CL_DisplayWindow window(desc);

  CL_GraphicContext gc = window.get_gc();

  CL_SoundOutput output(44100);

  // Create world
  World world(window);

  // Run the main loop
  world.run();

  return 0;
}

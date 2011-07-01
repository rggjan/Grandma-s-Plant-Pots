// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include <vector>

#include "./world.h"

int Application::main (const std::vector<CL_String> &args) {
  // Create a window
  CL_DisplayWindowDescription desc;
  desc.set_title ("Grandma's Plant Pots");
  // Use this resolution (as caption is disabled)
  desc.set_size(CL_Size(700, 700), true);

  CL_DisplayWindow window(desc);

  CL_GraphicContext gc = window.get_gc();

  CL_SoundOutput output (44100);

  try {
    // Create world
    World world (window);

    // Run the main loop
    world.run();
  } catch (CL_Exception &exception) {
    // Create a console window for text-output if not available
    CL_ConsoleWindow console ("Console", 80, 160);
    CL_Console::write_line ("Exception caught: " + exception.get_message_and_stack_trace());
    console.display_close_message();

    return -1;
  }

  return 0;
}

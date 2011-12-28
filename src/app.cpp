// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/sound.h>
#include <vector>

#include "./world.h"

using std::vector;

int Application::main(const std::vector<CL_String> &args) {
  // Create window
  CL_DisplayWindowDescription desc;
  desc.set_title("Grandma's Plant Pots");
  desc.set_allow_resize(true);
  desc.set_size(CL_Size(640, 480), true);
  CL_DisplayWindow *window = new CL_DisplayWindow(desc);

  // Maximise
  window->maximize();

  // Wait for window manager
  while (!window->is_maximized()) {
    CL_KeepAlive::process();
  }

  CL_KeepAlive::process();

  CL_SoundOutput output(44100);

  try {
    // Create world
    World world(window);

    // Run the main loop
    world.Run();
  } catch(CL_Exception exception) {
    // Create a console window for text-output if not available
    CL_ConsoleWindow console("Console", 80, 160);
    CL_Console::write_line("Exception caught: " +
                           exception.get_message_and_stack_trace());
    console.display_close_message();

    return -1;
  }

  return 0;
}

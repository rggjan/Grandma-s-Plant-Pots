// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include <vector>
#include <sstream>

#include "./world.h"

using std::vector;

int Application::main(const std::vector<CL_String> &args) {
  vector<CL_DisplayWindow*> windows;

  for (int i=0; i<4; i++) {
    // Create a window description
    CL_DisplayWindowDescription desc;
    std::ostringstream stream;
    stream << "Grandma's Plant Pots: Player " << i;
    desc.set_title(stream.str());

    // Use this resolution (as caption is disabled)
    //desc.set_size(CL_Size(500, 500), true);
    int window_width = 400;
    int window_height = 400;
    desc.set_position(CL_Rect(window_width*(i%2), window_height*(i/2),
                              window_width*(i%2)+window_width,
                              window_height*(i/2)+window_height), false);

    CL_DisplayWindow* window = new CL_DisplayWindow(desc);
    windows.push_back(window);
  }

  CL_SoundOutput output(44100);

  try {
    // Create world
    World world(windows);

    // Run the main loop
    world.run();
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

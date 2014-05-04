// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/sound.h>

#include <vector>
#include <string>

#include "./world.h"

using std::vector;

int Application::main(const std::vector<std::string> &args) {
  clan::ConsoleWindow consoleWindow("Whats up");

  // Create window
  clan::DisplayWindowDescription desc;
  desc.set_title("Grandma's Plant Pots");
  // desc.set_allow_resize(true);
  // desc.set_size(clan::Size(640, 480), true);
  desc.set_fullscreen(true);
  clan::DisplayWindow window(desc);

  clan::KeepAlive::process();

  clan::SoundOutput output(44100);

  try {
    // Create world
    World world(window);

    // Run the main loop
    world.Run();
  } catch(clan::Exception exception) {
    // Create a console window for text-output if not available
    clan::ConsoleWindow console("Console", 80, 160);
    clan::Console::write_line("Exception caught: " +
                           exception.get_message_and_stack_trace());
    console.display_close_message();

    return -1;
  }

  return 0;
}

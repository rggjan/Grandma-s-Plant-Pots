// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include <vector>

#include "./world.h"

#define NUM_PLAYERS 4

using std::vector;

void get_display_size(int* width, int* height, int* border) {
  // Create description
  CL_DisplayWindowDescription desc;
  desc.set_allow_resize(true);
  desc.set_position(CL_Rect(100, 100, 200, 200), true);

  // Calculate border
  CL_DisplayWindow window(desc);
  *border = window.get_geometry().top - 100;

  // Maximise
  window.maximize();

  // Wait for window manager
  while (!window.is_maximized()) {
    CL_KeepAlive::process();
  }

  // Calculate width, height
  *width = window.get_geometry().get_width();
  *height = window.get_geometry().get_height();
}

int Application::main(const std::vector<CL_String> &args) {
  vector<CL_DisplayWindow*> windows;
  windows.resize(NUM_PLAYERS);

  int width;
  int height;
  int border;
  get_display_size(&width, &height, &border);
  CL_Console::write_line("w: %1, h: %2, b: %3", width, height, border);

  int top_players = (NUM_PLAYERS+1)/2;
  int window_width = width/top_players;
  CL_Console::write_line("window_width %1", window_width);

  int window_height = height/2;

  for (int i = NUM_PLAYERS-1; i >= 0; i--) {
    // Create a window description
    CL_DisplayWindowDescription desc;
    desc.set_title(cl_format("GPP: Player %1", i));

    if (NUM_PLAYERS == 2) {
      desc.set_position(CL_Rect(i*width/2, 0, (i+1)*width/2, height), false);
    } else {
      if (i < top_players) {
        desc.set_position(CL_Rect(i*window_width, 0,
                                  (i+1)*window_width, window_height),
                          false);
      } else {
        desc.set_position(CL_Rect((i-top_players)*window_width,
                                  border+window_height,
                                  (i-top_players+1)*window_width,
                                  border+2*window_height),
                          false);
      }
    }

    CL_DisplayWindow* window = new CL_DisplayWindow(desc);
    windows[i] = window;
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

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./app.h"

#include <ClanLib/sound.h>
#include <vector>

#include "./world.h"

#define NUM_PLAYERS 4

#define CALCULATE_WINDOW_SIZES false

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
  CL_DisplayWindow* window;

  if (CALCULATE_WINDOW_SIZES) {
    windows.resize(NUM_PLAYERS);

    int width;
    int height;
    int border;
    get_display_size(&width, &height, &border);
    CL_Console::write_line("w: %1, h: %2, b: %3", width, height, border);

    int top_players = (NUM_PLAYERS + 1) / 2;
    int window_width = width / top_players;
    CL_Console::write_line("window_width %1", window_width);

    int window_height = height / 2;

    for (int i = NUM_PLAYERS - 1; i >= 0; i--) {
      // Create a window description
      CL_DisplayWindowDescription desc;
      desc.set_title(cl_format("GPP: Player %1", i));

      CL_Rect position;
      if (NUM_PLAYERS == 2) {
        position = CL_Rect(i * width / 2, 0, (i + 1) * width / 2, height);
      } else {
        if (i < top_players) {
          position = CL_Rect(i * window_width, 0,
                             (i + 1) * window_width, window_height);
        } else {
          position = CL_Rect((i - top_players) * window_width,
                             border + window_height,
                             (i - top_players + 1) * window_width,
                             border + 2 * window_height);
        }
      }
      desc.set_position(position, false);

      CL_DisplayWindow* window = new CL_DisplayWindow(desc);

      // TODO(rggjan): HACK to make it work...
      while (std::abs(desc.get_position().left -
                      window->get_geometry().left) > 1) {
        window->set_position(position, false);
        CL_KeepAlive::process();
      }
      windows[i] = window;
    }
  } else {
    //for (int i = 0; i < NUM_PLAYERS; i++) {
      // Create a window description
      CL_DisplayWindowDescription desc;
      desc.set_title(cl_format("GPP: Player %1", 1));

      // Use this resolution (as caption is disabled)
      // desc.set_size(CL_Size(500, 500), true);
      int window_width = 400;
      int window_height = 400;
      desc.set_position(CL_Rect(0, 0, 400, 400), false);

      window = new CL_DisplayWindow(desc);
    //}
  }

  CL_SoundOutput output(44100);

  try {
    // Create world
    World world(window, NUM_PLAYERS);

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

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./program.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/application.h>
#include <ClanLib/d3d.h>
#include <ClanLib/gl.h>
#include <ClanLib/swrender.h>

#include <vector>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#include "./app.h"

int Program::main(const std::vector<std::string> &args) {
  try {
    // Initialize ClanLib base components
    clan::SetupCore setup_core;

    // Initialize the ClanLib display component
    clan::SetupDisplay setup_display;

    clan::SetupD3D setup_d3d;
    clan::SetupGL setup_gl;
    clan::SetupSWRender setup_swrender;

    clan::SetupSound setup_sound;

    // Start the Application
    Application app;
    int retval = app.main(args);
    return retval;
  } catch(clan::Exception exception) {
    // Create a console window for text-output if not available
    clan::ConsoleWindow console("Console", 80, 160);
    clan::Console::write_line("Exception caught: " +
                           exception.get_message_and_stack_trace());
    console.display_close_message();

    return -1;
  }
}

// Instantiate clan::ClanApplication, informing it where the Program is located
clan::Application app(&Program::main);

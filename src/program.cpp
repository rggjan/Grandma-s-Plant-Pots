// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#include "./program.h"

#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/application.h>
//#include <ClanLib/vorbis.h>

// Choose the target renderer
#define USE_OPENGL_2
// #define USE_OPENGL_1
// #define USE_SOFTWARE_RENDERER

#ifdef USE_SOFTWARE_RENDERER
#include <ClanLib/swrender.h>
#endif

#ifdef USE_OPENGL_1
#include <ClanLib/gl1.h>
#endif

#ifdef USE_OPENGL_2
#include <ClanLib/gl.h>
#endif

#include <vector>

#include "./app.h"

int Program::main(const std::vector<std::string> &args) {
  try {
    // Initialize ClanLib base components
    clan::SetupCore setup_core;

    // Initialize the ClanLib display component
    clan::SetupDisplay setup_display;

#ifdef USE_SOFTWARE_RENDERER
    clan::SetupSWRender setup_swrender;
#endif

#ifdef USE_OPENGL_1
    clan::SetupGL1 setup_gl1;
#endif

#ifdef USE_OPENGL_2
    clan::SetupGL setup_gl;
#endif

    clan::SetupSound setup_sound;
    //clan::SetupVorbis setup_vorbis;

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

// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PROGRAM_H_
#define SRC_PROGRAM_H_

#include <ClanLib/core.h>
#include <ClanLib/sound.h>

#include <vector>

// This is the Program class that is called by CL_ClanApplication
class Program {
  public:
    static int main(const std::vector<CL_String> &args);
};

#endif  // SRC_PROGRAM_H_

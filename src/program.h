// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_PROGRAM_H_
#define SRC_PROGRAM_H_

#include <ClanLib/core.h>
#include <ClanLib/core.h>

#include <vector>
#include <string>

// This is the Program class that is called by clan::ClanApplication
class Program {
  public:
    static int main(const std::vector<std::string> &args);
};

#endif  // SRC_PROGRAM_H_

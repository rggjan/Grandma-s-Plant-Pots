// Copyright 2011 Jan Rüegg <rggjan@gmail.com>

#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <ClanLib/core.h>

#include <vector>

class Application {
  public:
    Application() {}
    virtual ~Application() {}

    virtual int main(const std::vector<CL_String> &args);
};

#endif  // SRC_APP_H_

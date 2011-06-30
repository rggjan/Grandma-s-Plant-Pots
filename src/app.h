#pragma once

#include <vector>
#include <ClanLib/core.h>

class Application
{
public:
	Application() {};

	virtual int main(const std::vector<CL_String> &args);
};


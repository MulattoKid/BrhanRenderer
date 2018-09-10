#include "BrhanSystem.h"
#include <cstdlib>
#include "Logger.h"

BrhanSystem::BrhanSystem(const int argc, char** argv)
{
	if (argc != 5)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "4 arguments must be passed, missing %i\n", 5 - argc);
	}
	
	render_width = std::atoi(argv[1]);
	render_height = std::atoi(argv[2]);
	spp = std::atoi(argv[3]);
	render_file = argv[4];
}

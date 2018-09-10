#include "BrhanSystem.h"
#include <cstdlib>
#include "Logger.h"
#include <string>

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

void BrhanSystem::UpdateProgress(unsigned int y) const
{
	unsigned int total_effort = render_width * render_height;
	unsigned int progress = (unsigned int)((render_width * y) / float(total_effort) * 100.0f);
	std::string output = "";
	for (unsigned int i = 0; i < progress; i++)
	{
		output += "-";
	}
	for (unsigned int i = progress; i < 100; i++)
	{
		output += " ";
	}
	LOG_MESSAGE(true, "\rRender progress (%lu\%): [%s]", progress, output.c_str());
}

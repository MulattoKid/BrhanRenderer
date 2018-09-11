#include "BrhanSystem.h"
#include <cstdlib>
#include "DirectLightingIntegrator.h"
#include "Logger.h"
#include "PathIntegrator.h"
#include <string>

BrhanSystem::BrhanSystem(const int argc, char** argv)
{
	const unsigned int num_args = 7;
	const unsigned int arg_render_width = 1;
	const unsigned int arg_render_height = 2;
	const unsigned int arg_spp = 3;
	const unsigned int arg_depth = 4;
	const unsigned int arg_integrator = 5;
	const unsigned int arg_file = 6;

	if (argc != num_args)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "%lu arguments must be passed\n", num_args - 1);
	}
	
	render_width = std::atoi(argv[arg_render_width]);
	render_height = std::atoi(argv[arg_render_height]);
	spp = std::atoi(argv[arg_spp]);
	depth = std::atoi(argv[arg_depth]);
	
	if (std::strcmp(argv[arg_integrator], "direct") == 0)
	{
		integrator = new DirectLightingIntegrator();
		integrator_type = DIRECT_LIGHTING_INTEGRATOR;
	}
	else if (std::strcmp(argv[arg_integrator], "path") == 0)
	{
		integrator = new PathIntegrator();
		integrator_type = PATH_INTEGRATOR;
	}
	else
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Integrator '%s' is not supported\n", argv[arg_integrator]);
	}
	
	render_file = argv[arg_file];
}

BrhanSystem::~BrhanSystem()
{
	if (integrator_type == IntegratorType::DIRECT_LIGHTING_INTEGRATOR)
	{
		DirectLightingIntegrator* ptr = (DirectLightingIntegrator*)(integrator);
		delete ptr;
	}
}

void BrhanSystem::UpdateProgress(unsigned int y) const
{
	unsigned int total_effort = render_width * render_height;
	unsigned int progress = (unsigned int)((render_width * y) / float(total_effort) * 100.0f);
	std::string output;
	output.resize(100, ' ');
	for (unsigned int i = 0; i < progress; i++)
	{
		output[i] = '-';
	}
	LOG_MESSAGE(true, "\rRender progress (%lu\%): [%s]", progress, output.c_str());
}

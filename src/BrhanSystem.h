#ifndef BRHANSYSTEM_H
#define BRHANSYSTEM_H

#include "Integrator.h"

struct BrhanSystem
{
	unsigned int render_width;
	unsigned int render_height;
	unsigned int spp;
	char* render_file;
	Integrator* integrator;
	IntegratorType integrator_type;
	
	BrhanSystem(const int argc, char** argv);
	~BrhanSystem();
	void UpdateProgress(unsigned int y) const;
};

#endif

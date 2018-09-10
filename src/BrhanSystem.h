#ifndef BRHANSYSTEM_H
#define BRHANSYSTEM_H

struct BrhanSystem
{
	unsigned int render_width;
	unsigned int render_height;
	unsigned int spp;
	char* render_file;
	
	BrhanSystem(const int argc, char** argv);
};

#endif

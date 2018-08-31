#ifndef MATERIAL_H
#define MATERIAL_H

#include "SurfaceInteraction.h"

struct Material
{
	virtual void ComputeScatteringFunctions(SurfaceInteraction* isect) const = 0;
};

#endif

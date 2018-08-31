#include "BSDF.h"
#include "Shape.h"
#include "SurfaceInteraction.h"

SurfaceInteraction::~SurfaceInteraction()
{
	if (bsdf != NULL)
	{
		delete bsdf;
	}
}

void SurfaceInteraction::ComputeScatteringFunctions()
{
	shape->ComputeScatteringFunctions(this);
}

#include "BSDF.h"
#include "Shape.h"
#include "SurfaceInteraction.h"

void SurfaceInteraction::ComputeScatteringFunctions()
{
	shape->ComputeScatteringFunctions(this);
}

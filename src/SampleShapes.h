#ifndef SAMPLESHAPES_H
#define SAMPLESHAPES_H

#include "RNG.h"

glm::vec3 UniformSampleUnitDisk(const float u[2]);
glm::vec3 UniformSampleUnitDisk(RNG& rng);

#endif

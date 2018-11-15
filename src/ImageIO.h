#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "BrhanSystem.h"

void WriteImage(const float* film, const BrhanSystem& system, const unsigned int interval = 0);
void WriteDepthImage(const float* film, const BrhanSystem& system);

#endif

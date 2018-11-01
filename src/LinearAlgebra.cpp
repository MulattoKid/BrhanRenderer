#include <cmath>
#include "LinearAlgebra.h"

//p.1080
bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float* x0, float* x1)
{
	float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
	if (std::abs(det) < 1e-10f)
	{
		return false;
	}
	
	*x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
	*x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
	
	if (std::isnan(*x0) || std::isnan(*x1))
	{
		return false;
	}
	return true;
}

#include "FresnelNoOp.h"

glm::vec3 FresnelNoOp::Evaluate(const glm::vec3& wi, const glm::vec3& normal) const
{
	return glm::vec3(1.0f);
}

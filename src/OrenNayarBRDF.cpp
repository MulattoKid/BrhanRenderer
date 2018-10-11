#include "GeometricUtilities.h"
#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "OrenNayarBRDF.h"

//p.535
//http://www1.cs.columbia.edu/CAVE/publications/pdfs/Oren_SIGGRAPH94.pdf
//Sigma is expected to be given in degrees
OrenNayarBRDF::OrenNayarBRDF(const glm::vec3& R, const float sigma) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE))
{
	this->R = R;
	const float sigma2 = glm::pow(glm::radians(sigma), 2);
	this->A = 1.0f - (sigma2 / (2.0f * (sigma2 + 0.33f)));
	this->B = (0.45f * sigma2) / (sigma2 + 0.09f);
}

OrenNayarBRDF::~OrenNayarBRDF()
{}

//p.535
glm::vec3 OrenNayarBRDF::f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const
{
	const float theta_o = glm::acos(glm::dot(normal, wo));
	const float theta_i = glm::acos(glm::dot(normal, wi));

	//Steps to find phi_i and phi_o
	//1) I say that the y-axis should be aligned with the normal, thus giving the rotation
	//		necessary to find the x-axis in the local space
	const glm::mat3 rotation = RotationToAlignAToB(glm::vec3(0.0f, 1.0f, 0.0f), normal);
	const glm::vec3 transformed_x_axis = glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
	//2) I need to project wi/o onto the xz-plane, as I've decided the normal is the y-axis
	const glm::vec3 proj_wo = ProjectVectorOntoPlane(wo, normal);
	const glm::vec3 proj_wi = ProjectVectorOntoPlane(wi, normal);
	const float phi_o = glm::acos(glm::dot(transformed_x_axis, proj_wo));
	const float phi_i = glm::acos(glm::dot(transformed_x_axis, proj_wi));
	
	const float alpha = glm::max(theta_i, theta_o);
	const float beta  = glm::min(theta_i, theta_o);
	
	return (R / glm::pi<float>()) * (A + (B * glm::max(0.0f, glm::cos(phi_i - phi_o)) * glm::sin(alpha) * glm::tan(beta)));
}

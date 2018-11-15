#include "BSDF.h"
#include "FresnelConductor.h"
#include "FresnelDielectric.h"
#include "FresnelNoOp.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "LambertianBRDF.h"
#include "Logger.h"
#include "OrenNayarBRDF.h"
#include "RNG.h"
#include "SpecularBRDF.h"
#include "SpecularBTDF.h"

BSDF::BSDF()
{}

BSDF::~BSDF()
{	
	for (int i = 0; i < num_bxdfs; i++)
	{
		delete bxdfs[i];
	}
}

void BSDF::Add(BxDF* bxdf)
{
	if (num_bxdfs == max_bxdfs)
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "The maximum number of BxDFs (%i) is already reached - skipping and deleting the supplied BxDF\n", max_bxdfs);
		delete bxdf;
		return;
	}
	bxdfs[num_bxdfs++] = bxdf;
}

int BSDF::NumMatchingComponents(const BxDFType type) const 
{
	int matching = 0;
	for (int i = 0; i < num_bxdfs; i++)
	{
		if (bxdfs[i]->MatchesFlags(type))
		{
			matching++;
		}
	}
	
	return matching;
}

float BSDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal, BxDFType flags)
{
	int matching_comp = 0;
	float pdf = 0.0f;
	for (int i = 0; i < num_bxdfs; i++)
	{
		if (bxdfs[i]->MatchesFlags(flags))
		{
			pdf += bxdfs[i]->Pdf(wo, wi, normal);
			matching_comp++;
		}
	}
	
	return matching_comp > 1 ? pdf / float(matching_comp) : pdf;
}

glm::vec3 BSDF::f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi, BxDFType flags) const
{
	glm::vec3 f(0.0f);
	bool reflect = glm::dot(wo, normal) * glm::dot(wi, normal) > 0.0f;
	for (int i = 0; i < num_bxdfs; i++)
	{
		if (bxdfs[i]->MatchesFlags(flags) &&
			((reflect  && (bxdfs[i]->type & BSDF_REFLECTION)) ||
			 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
		{
			f += bxdfs[i]->f(wo, normal, wi);
		}
	}
	
	return f;
}

glm::vec3 BSDF::Samplef(RNG& rng, const glm::vec3& wo, float u[2], const BxDFType type, const glm::vec3& normal,
						glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	const int num_matching = NumMatchingComponents(type);
	if (num_matching == 0) //None of the BxDFs in making up the BSDF match the requested type
	{
		*pdf = 0.0f;
		return glm::vec3(0.0f);
	}
	
	//Randomly choose and find one of the matching BxDFs
	const int bxdf_to_sample = glm::min(int(u[0] * num_matching), num_matching - 1);
	BxDF* bxdf = NULL;
	int count = bxdf_to_sample;
	for (int i = 0; i < num_bxdfs; i++)
	{
		if (bxdfs[i]->MatchesFlags(type) && count-- == 0)
		{
			bxdf = bxdfs[i];
			break;
		}
	}
	
	//Resample u[0] as it was used to choose the BxDF
	rng.Uniform1D(&u[0]);
	
	//Sample BxDF to get wi
	glm::vec3 f = bxdf->Samplef(wo, u, normal, wi, pdf, sampled_type);
	
	//Account for the other BxDFs that could have been chosen
	if (num_matching > 1 && !(bxdf->type & BSDF_SPECULAR))
	{
		for (int i = 0; i < num_bxdfs; i++)
		{
			if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type))
			{
				*pdf += bxdfs[i]->Pdf(wo, *wi, normal);
			}
		}
	}
	if (num_matching > 1)
	{	
		*pdf /= num_matching;
	}
	
	//Compute value for BSDF for the sampled direction
	if (num_matching > 1 && !(bxdf->type & BSDF_SPECULAR))
	{
		f = this->f(wo, normal, *wi, type);
	}
	
	return f;
}

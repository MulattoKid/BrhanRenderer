#include "Shape.h"

glm::vec3 Shape::AmbientColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->ambient;
}

glm::vec3 Shape::DiffuseColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->diffuse;
}

glm::vec3 Shape::SpecularColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->specular;
}

glm::vec3 Shape::EmissionColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->emission;
}

bool Shape::IsAreaLight() const
{
	return area_light == NULL ? false : true;
}

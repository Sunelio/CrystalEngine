#include "Resources/Material.h"
using namespace Resources;
using namespace Maths;

Material::Material(const std::string& _name)
{
    name = _name;
    type = ResourceType::Material;
}

void Material::Load()
{
    loaded = true;
}

void Material::SendToPipeline()
{
	sentToPipeline = true;
}

void Material::SetParams(const RGB& _ambient, const RGB& _diffuse, const RGB& _specular, const RGB& _emission, const float& _shininess, const float& _transparency)
{
    ambient      = _ambient;
    diffuse      = _diffuse;
    specular     = _specular;
    emission     = _emission;
    shininess    = _shininess;
    transparency = _transparency;
}

std::vector<Texture**> Material::GetTextures()
{
    return { &ambientTexture, &diffuseTexture, &specularTexture, &emissionTexture, &shininessMap, &alphaMap, &normalMap };
}

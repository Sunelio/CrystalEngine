#pragma once
#include <vector>

#include "Resources/Resource.h"
#include "Maths/Color.h"

namespace Resources
{
    class Texture;

    // - Material (inherits from Resource) - //
    class Material : public Resource
    {
    public:
        // -- Attributes -- //
        Maths::RGB ambient, diffuse, specular; // Values that influence incoming light color.
        Maths::RGB emission;     // The color of light emitted by the object.
        float shininess    = 32; // Defines the intensity of highlights on the object.
        float transparency = 1;  // Defines how see-through the object is.

        Texture* ambientTexture  = nullptr; // Texture used for the color of ambient light.
        Texture* diffuseTexture  = nullptr; // Texture used for the color of diffuse light.
        Texture* specularTexture = nullptr; // Texture used for the color of specular light.
        Texture* emissionTexture = nullptr; // Texture used for the color of light emitted by the object.
        Texture* shininessMap    = nullptr; // Texture used to modify the object's shininess.
        Texture* alphaMap        = nullptr; // Texture used to modify the object's transparency.
        Texture* normalMap       = nullptr; // Texture used to modify the object's normals.

        // -- Constructor -- //
        
        // Creates a material with the given name (usually a filename).
        Material(const std::string& _name); 

        // -- Methods -- //
        void Load()           override; // Loads the material.
        void SendToPipeline() override; // Sends the material to GPU memory.
        
        // Sets the material's parameters to the given ones.
        void SetParams(const Maths::RGB& _ambient, const Maths::RGB& _diffuse, const Maths::RGB& _specular, const Maths::RGB& _emission, const float& _shininess, const float& _transparency);

        std::vector<Texture**> GetTextures();
        
        static ResourceType GetResourceType() { return ResourceType::Material; }
    };
}

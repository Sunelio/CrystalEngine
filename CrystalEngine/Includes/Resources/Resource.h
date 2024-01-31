#pragma once
#include <string>

namespace Resources
{
    // - ResourceType - //
    enum class ResourceType
    {
        Undefined,
        Texture,
        RenderTexture,
        Cubemap,
        Material,
        Mesh,
        VertexShader,
        FragmentShader,
        ShaderProgram,
        Font,
        Sound,
    };
    static std::string ResourceTypeToString(const ResourceType& type);

    // - Resource: Parent class for all engine resources - //
    class Resource
    {
    protected:
        std::string  name;
        ResourceType type = ResourceType::Undefined;

        bool loaded         = false;
        bool sentToPipeline = false;
        int  usageCounter   = 0;

    protected:
        Resource() = default;
        
    public:
        virtual ~Resource()                  = default;
        Resource(const Resource&)            = delete;
        Resource(Resource&&)                 = delete;
        Resource& operator=(const Resource&) = delete;
        Resource& operator=(Resource&&)      = delete;

        // -- Methods -- //
        virtual void Load()           = 0; // Overriden by child resources to be loaded.
        virtual void SendToPipeline() = 0; // Overriden by child resources to be sent to GPU memory.

        // -- Getters -- //
        std::string  GetName()   const { return name; } // Returns the resource's name.
        ResourceType GetType()   const { return type; } // Returns the resource's type.
        bool IsLoaded()          const { return loaded;         } // Returns true if the resource is loaded.
        bool WasSentToPipeline() const { return sentToPipeline; } // Returns true if the resource has been sent to the pipeline.
        bool IsUsed()            const { return usageCounter > 0; } // Returns true if the resource still has a use, false if it can be unloaded.

        // -- Setters -- //
        void SetLoadingDone() { loaded = true; }
        
        void AddUser()            { usageCounter++; }
        void DelUser()            { usageCounter--; }
        int  GetUserCount() const { return usageCounter; }
    };
}

// - Non-member functions - //

// Returns the name of the given resource type as a string.
inline std::string Resources::ResourceTypeToString(const ResourceType& type)
{
    switch (type)
    {
    case ResourceType::Texture:
        return "Texture";
    case ResourceType::RenderTexture:
        return "Render Texture";
    case ResourceType::Cubemap:
        return "Cubemap";
    case ResourceType::Material:
        return "Material";
    case ResourceType::Mesh:
        return "Mesh";
    case ResourceType::VertexShader:
        return "Vertex Shader";
    case ResourceType::FragmentShader:
        return "Fragment Shader";
    case ResourceType::ShaderProgram:
        return "Shader Program";
    case ResourceType::Font:
        return "Font";
    case ResourceType::Sound:
        return "Sound";
    case ResourceType::Undefined:
    default:
        return "Undefined";
    }
}

#pragma once

#include "Maths/Color.h"
#include "Resource.h"
#include "Maths/Vector2.h"
#include "Utils/PipelineData.h"

namespace Render { class Renderer; }
namespace Resources
{
    // - RenderTexture (inherits from Resource) - //
    class RenderTexture : public Resource
    {
    private:
        Render::Renderer* renderer = nullptr;
        int width = 0, height = 0;
        Utils::RenderTexturePipelineData pipelineData;

    public:
        // -- Attributes -- //
        Maths::RGBA clearColor; // The color to fill the texture to.

        // -- Constructor -- //
        
        // Creates a render texture with the given name.
        RenderTexture(const std::string& _name);
        ~RenderTexture() override;
        RenderTexture(const RenderTexture&)            = delete;
        RenderTexture(RenderTexture&&)                 = delete;
        RenderTexture& operator=(const RenderTexture&) = delete;
        RenderTexture& operator=(RenderTexture&&)      = delete;

        // -- Methods -- //
        void Load()           override; // Loads the render texture.
        void SendToPipeline() override; // Sends the render texture to GPU memory.

        void BeginRender() const; // Starts rendering onto the texture.
        void EndRender()   const; // Stops rendering to the texture and starts drawing objects directly to the screen texture.

        // -- Setters -- //
        void SetWidth (const int& _width );                          // Modifies the render texture's width.
        void SetHeight(const int& _height);                          // Modifies the render texture's height.
        void SetSize  (const int& _width, const int& _height);       // Modifies the render texture's size.
		void SetPipelineData(const Utils::RenderTexturePipelineData& data);

        // -- Getters -- //
        int            GetWidth () const { return width;                           }      // Returns the render texture's width.
        int            GetHeight() const { return height;                          }      // Returns the render texture's height.
        Maths::Vector2 GetSize  () const { return { (float)width, (float)height }; }      // Returns the render texture's size.
        Utils::RenderTexturePipelineData GetPipelineData() const { return pipelineData; } // Returns the render texture's pipeline data.
        static ResourceType GetResourceType() { return ResourceType::RenderTexture; }     // Returns the resource type of this class.
    };
}

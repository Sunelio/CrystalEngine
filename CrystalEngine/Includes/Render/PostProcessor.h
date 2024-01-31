#pragma once
#include "../Externals/Includes/LightJson/JsonVirtual.h"
#include <vector>

namespace Scripts
{
    struct ScreenShaderScript;
}

namespace Resources
{
    class RenderTexture;
    class FragmentShader;
    class ShaderProgram;
}
namespace Maths { class RGBA; }
namespace Render
{    
    class Renderer;

    // - PostProcessor: applies post-processing effects to the screen texture - //
    class PostProcessor : public LightJson::JsonVirtual
    {
    private:
        Renderer*                 renderer        = nullptr;
        Resources::RenderTexture* pingPongTexture = nullptr;
        std::vector<Resources::ShaderProgram*> shaders;

    public:
        PostProcessor();
        ~PostProcessor();
        PostProcessor(const PostProcessor&)            = delete;
        PostProcessor(PostProcessor&&)                 = delete;
        PostProcessor& operator=(const PostProcessor&) = delete;
        PostProcessor& operator=(PostProcessor&&)      = delete;

        void BeginRender(const Resources::RenderTexture* renderTexture) const; // Begins rendering on the screen textures.
        void EndRender  (      Resources::RenderTexture* renderTexture) const; // Applies post-processing to the screen texture.

        void AddShader        (Resources::FragmentShader* shader, Scripts::ScreenShaderScript* script = nullptr); // Adds the given shader to the post-processing shaders.
        void AddShaderProgram (Resources::ShaderProgram* shaderProgram); // Adds the given shader to the post-processing shaders.
        void AddShaderPrograms(size_t count, ...); // Adds the given shader to the post-processing shaders.

        void RemoveShaderProgram(const Resources::ShaderProgram* shaderProgram);

        std::vector<Resources::ShaderProgram*> GetShaders() const { return shaders; }

        LightJson::JsonElement* Serialize() const                   override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}
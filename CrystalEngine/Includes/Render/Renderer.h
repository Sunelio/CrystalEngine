#pragma once
#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Utils/PipelineData.h"

namespace Resources
{
    enum class ResourceType;
    class Cubemap;
    class Character;
    class Material;
    class ShaderProgram;
    class Texture;
    class RenderTexture;
    class SubMesh;
    class AnimatedSubMesh;
    class StaticSubMesh;
    class Font;
}
namespace Maths
{
    class RGBA;
    class RGB;
    class Vector2;
    template<int R, int C> class Matrix;
    typedef Matrix<4, 4> Mat4;
}
namespace Render
{
    class LightManager;
    class Light;
    class Camera;

    // - Renderer: Interface to the rendering API - //
    class Renderer
    {
    private:
        bool renderObjectsToStencil = false;
        uint textureSampler         = 0;
        Utils::ScreenRectPipelineData screenRect;
        Utils::ScreenRectPipelineData uiRect;
    
    public:
        Renderer();
        ~Renderer();
        Renderer(const Renderer&)            = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&)      = delete;

        // -- Sending resources to GPU memory -- //
        template <typename T> void SendResourceToPipeline(T* resource) const; // Stores the given resource into GPU memory.
        template <typename T> void DeleteResource        (T* resource) const; // Removes the given resource's data from GPU memory.
        uint CreateShaderBuffer() const; // Creates a shader buffer and returns it.
        void SetShaderStorageBufferData(const uint& id, const size_t& size, const void* data) const; // Modifies the data stored in the given buffer to the given data.
        void SetShaderArrayBufferData  (const uint& id, const size_t& size, const void* data) const; // Modifies the data stored in the given buffer to the given data.
        void DeleteShaderBuffer(const uint& id) const; // Deletes the given buffer from GPU memory.

        // -- Handling Shaders -- //
        void UseShaderProgram (const Resources::ShaderProgram* shaderProgram) const; // Starts using the given shader program.
        void SetShaderSampler (const Resources::ShaderProgram* shaderProgram, const char* name, const uint& location, const uint& textureID) const; // Sets shader texture sampler variable.
        int  GetShaderVariableLocation(const Resources::ShaderProgram* shaderProgram, const char* name) const; // Returns the location of the given variable in the given shader program.
        template <typename T> void SetShaderVariable(const Resources::ShaderProgram* shaderProgram, const char* name, const T& val) const; // Sets the value of the specified shader variable.
        template <typename T> void SetShaderVariable(const int& location, const T& val) const; // Sets the value of the shader variable at the specified location.

        // -- RenderTextures -- //
        void BindRenderTexture(const Resources::RenderTexture* renderTexture) const; // Starts using the given render texture to draw objects.
        void UnbindRenderTexture() const;                                            // Starts drawing objects directly to the screen texture.
        void ClearBindedRenderTexture(const Maths::RGBA& color, const bool& onlyStencil = false) const; // Fills the currently binded render texture with the given color.
        
        // -- Rendering meshes -- //
        void DrawSubMesh  (const Resources::SubMesh* subMesh,                           const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId = 0) const; // Renders the given sub-mesh using the specified model transform matrix, camera and the light manager.
        void DrawInstanced(const Resources::SubMesh* subMesh, const int& instanceCount, const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId = 0) const; // Renders the given instanced sub-mesh using the specified model transform matrix, camera and the light manager.
        void DrawCubemap  (const Resources::SubMesh* subMesh, const Resources::Cubemap* cubemap, const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId = 0) const; // Renders the given cubemap using the specified model transform matrix, camera and the light manager.

        // -- Rendering 2D textures --//
        void DrawScreenTexture(const Resources::Texture*       texture, const Resources::ShaderProgram* shaderProgram = nullptr) const; // Render the given texture on the screen.
        void DrawScreenTexture(const Resources::RenderTexture* texture, const Resources::ShaderProgram* shaderProgram = nullptr) const; // Render the given texture on the screen.
        void DrawText         (const std::string& text, const Resources::Font* font, const Maths::Vector2& pos, const Maths::Vector2& scale, const Maths::RGBA& color, const size_t& id = 0) const; // Renders the given text with the given font.
        void DrawTexture2D    (const Resources::Texture* texture, const Maths::Vector2& size, const Maths::Vector2& pos, const Maths::Vector2& texSize, const size_t& id = 0) const; // Draws the given texture on the screen.
        void DrawLine         (const Maths::Vector2& start, const Maths::Vector2& end, const Maths::RGB& startColor, const Maths::RGB& endColor) const; // Draws a 2D line.
        void DrawLine         (const Maths::Vector3& start, const Maths::Vector3& end, const Maths::RGB& startColor, const Maths::RGB& endColor, const Camera* camera) const; // Draws a 3D line.
        
        // -- Miscellaneous -- //
        uint   GetTextureSamplerId() const { return textureSampler; } // Returns the ID of the texture sampler (necessary to sample textures within shaders).
        void   RenderObjectsToStencil(const bool& enable);            // Enables drawing object IDs to a texture to make it possible to click on viewport objects.
        void   RenderWireframe       (const bool& enable) const;      // Starts rendering everything in wireframe mode when set to true, to revert, set it to false.
        size_t PickObjectFromViewport(const Maths::Vector2& pickPos, const Resources::RenderTexture* viewportTexture, const Maths::Vector2& viewportSize) const; // Returns the ID of the clicked object, 0 if no object is clicked and negative values if the gizmo is clicked.

    private:
        void CompileShader(const uint& id, const Resources::ResourceType& type, const char* curFile, const char* curFunction, const long& curLine) const;
        bool PrepareDraw(const Resources::SubMesh* subMesh, const Maths::Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId, const bool& instanced = false) const;
    };
}

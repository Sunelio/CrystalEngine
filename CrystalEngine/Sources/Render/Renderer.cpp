#include "Render/Renderer.h"
#include "Render/PostProcessor.h"
#include "Render/Camera.h"
#include "Render/LightManager.h"
#include "Core/Window.h"
#include "Debug/Logger.h"
#include "Resources/Resource.h"
#include "Resources/Material.h"
#include "Resources/SubMesh.h"
#include "Resources/RenderTexture.h"
#include "Resources/Shader.h"
#include "Resources/ShaderProgram.h"
#include "Resources/Texture.h"
#include "Resources/Cubemap.h"
#include "Resources/ResourceManager.h"
#include "Scripts/ShaderScript.h"
#include "Maths/Color.h"
#include "Core/Engine.h"
#include "Maths/Maths.h"
#include "Resources/Font.h"
#include "Core/EventDispatcher.h"
#include "Animation/Skeleton.h"
#include <glad/glad.h>
#include <stbi/stb_image.h>
using namespace Core;
using namespace Render;
using namespace Debug;
using namespace Resources;
using namespace Maths;
using namespace Scripts;
using namespace Utils;

#pragma region SetShaderVariable

// ---------- SetShaderVariable ---------- //

template <typename T> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const T& val) const
{
    DebugLogError("The given variable type cannot be set to a shader uniform variable.");
}
template <typename T> void Renderer::SetShaderVariable(const int& location, const T& val) const
{
    DebugLogError("The given variable type cannot be set to a shader uniform variable.");
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const bool& val) const
{
    glUniform1i(glGetUniformLocation(shaderProgram->GetId(), name), val);
}
template <> void Renderer::SetShaderVariable(const int& location, const bool& val) const
{
    glUniform1i(location, val);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const int& val) const
{
    glUniform1i(glGetUniformLocation(shaderProgram->GetId(), name), val);
}
template <> void Renderer::SetShaderVariable(const int& location, const int& val) const
{
    glUniform1i(location, val);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const uint& val) const
{
    glUniform1ui(glGetUniformLocation(shaderProgram->GetId(), name), val);
}
template <> void Renderer::SetShaderVariable(const int& location, const uint& val) const
{
    glUniform1ui(location, val);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const float& val) const
{
    glUniform1f(glGetUniformLocation(shaderProgram->GetId(), name), val);
}
template <> void Renderer::SetShaderVariable(const int& location, const float& val) const
{
    glUniform1f(location, val);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Vector2& val) const
{
    glUniform2f(glGetUniformLocation(shaderProgram->GetId(), name), val.x, val.y);
}
template <> void Renderer::SetShaderVariable(const int& location, const Vector2& val) const
{
    glUniform2f(location, val.x, val.y);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Vector3& val) const
{
    glUniform3f(glGetUniformLocation(shaderProgram->GetId(), name), val.x, val.y, val.z);
}
template <> void Renderer::SetShaderVariable(const int& location, const Vector3& val) const
{
    glUniform3f(location, val.x, val.y, val.z);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Vector4& val) const
{
    glUniform4f(glGetUniformLocation(shaderProgram->GetId(), name), val.x, val.y, val.z, val.w);
}
template <> void Renderer::SetShaderVariable(const int& location, const Vector4& val) const
{
    glUniform4f(location, val.x, val.y, val.z, val.w);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const RGB& val) const
{
    glUniform3f(glGetUniformLocation(shaderProgram->GetId(), name), val.r, val.g, val.b);
}
template <> void Renderer::SetShaderVariable(const int& location, const RGB& val) const
{
    glUniform3f(location, val.r, val.g, val.b);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const RGBA& val) const
{
    glUniform4f(glGetUniformLocation(shaderProgram->GetId(), name), val.r, val.g, val.b, val.a);
}
template <> void Renderer::SetShaderVariable(const int& location, const RGBA& val) const
{
    glUniform4f(location, val.r, val.g, val.b, val.a);
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Mat2& val) const
{
    glUniformMatrix2fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Mat2& val) const
{
    glUniformMatrix2fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Mat3& val) const
{
    glUniformMatrix3fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Mat3& val) const
{
    glUniformMatrix3fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Mat4& val) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Mat4& val) const
{
    glUniformMatrix4fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<2, 3>& val) const
{
    glUniformMatrix2x3fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<2, 3>& val) const
{
    glUniformMatrix2x3fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<3, 2>& val) const
{
    glUniformMatrix3x2fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<3, 2>& val) const
{
    glUniformMatrix3x2fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<2, 4>& val) const
{
    glUniformMatrix2x4fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<2, 4>& val) const
{
    glUniformMatrix2x4fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<4, 2>& val) const
{
    glUniformMatrix4x2fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<4, 2>& val) const
{
    glUniformMatrix4x2fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<3, 4>& val) const
{
    glUniformMatrix3x4fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<3, 4>& val) const
{
    glUniformMatrix3x4fv(location, 1, GL_FALSE, val.AsPtr());
}

template <> void Renderer::SetShaderVariable(const ShaderProgram* shaderProgram, const char* name, const Matrix<4, 3>& val) const
{
    glUniformMatrix4x3fv(glGetUniformLocation(shaderProgram->GetId(), name), 1, GL_FALSE, val.AsPtr());
}
template <> void Renderer::SetShaderVariable(const int& location, const Matrix<4, 3>& val) const
{
    glUniformMatrix4x3fv(location, 1, GL_FALSE, val.AsPtr());
}
#pragma endregion

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::stringstream sstream;
    sstream << "Debug message (id " << id << "): " << message << "\n";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             sstream << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sstream << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sstream << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     sstream << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     sstream << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           sstream << "Source: Other"; break;
    default:                              sstream << "Source: Unknown"; break;
    } sstream << "\n";

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               sstream << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: sstream << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  sstream << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         sstream << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         sstream << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              sstream << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          sstream << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           sstream << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               sstream << "Type: Other"; break;
    default:                                sstream << "Type: Unknown"; break;
    } sstream << "\n";

    LogType logType;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         sstream << "Severity: High";         logType = LogType::Error;   break;
    case GL_DEBUG_SEVERITY_MEDIUM:       sstream << "Severity: Medium";       logType = LogType::Error;   break;
    case GL_DEBUG_SEVERITY_LOW:          sstream << "Severity: Low";          logType = LogType::Warning; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: sstream << "Severity: Notification"; logType = LogType::Info;    break;
    default:                             sstream << "Severity: Unknown";      logType = LogType::Info;    break;
    }

    DebugLogWithType(logType, sstream.str());
}

Renderer::Renderer()
{
    // Load all OpenGL function pointers from GLAD.
    Assert(gladLoadGLLoader(Window::GetProcAddressFuncPointer()), "Failed to initialize GLAD.");

    // Setup OpenGL flags.
    GLint flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);
    DebugLogInfo("Initialized OpenGL.");

    // Generate the texture sampler.
    glGenSamplers(1, &textureSampler);
    glSamplerParameteri(textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(textureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(textureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameterf(textureSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

    // Generate the screen rectangle.
    constexpr float rectangleVertices[] = 
    {
        // Coords     // texCoords
        1.0f, -1.0f,  1.0f, 0.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
       -1.0f,  1.0f,  0.0f, 1.0f,

        1.0f,  1.0f,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
       -1.0f,  1.0f,  0.0f, 1.0f
    };
    glGenVertexArrays(1, &screenRect.VAO);
    glGenBuffers(1, &screenRect.VBO);
    glBindVertexArray(screenRect.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenRect.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenVertexArrays(1, &uiRect.VAO);
    glGenBuffers(1, &uiRect.VBO);
    glBindVertexArray(uiRect.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiRect.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Setup the screen resize event callback.
    Engine::Get()->GetEventDispatcher()->AddListener<ScreenResizeEvent>([this](Event* event)
    {
        const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
        glViewport(0, 0, resizeEvent->width, resizeEvent->height);
    });
}

Renderer::~Renderer()
{
    glDeleteBuffers     (1, &screenRect.VBO);
    glDeleteVertexArrays(1, &screenRect.VAO);
    glDeleteSamplers    (1, &textureSampler);
}

uint Renderer::CreateShaderBuffer() const
{
    uint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void Renderer::SetShaderStorageBufferData(const uint& id, const size_t& size, const void* data) const
{
    glBindBuffer   (GL_SHADER_STORAGE_BUFFER, id);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, (int)size, data);
    glBindBuffer   (GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::SetShaderArrayBufferData(const uint& id, const size_t& size, const void* data) const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, (int)size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::DeleteShaderBuffer(const uint& id) const
{
    glDeleteBuffers(1, &id);
}

void Renderer::UseShaderProgram(const ShaderProgram* shaderProgram) const
{
    glUseProgram(shaderProgram->GetId());
}

void Renderer::SetShaderSampler(const ShaderProgram* shaderProgram, const char* name, const uint& location, const uint& textureID) const
{
    glBindTextureUnit(location, textureID);
    glBindSampler    (location, textureSampler);
    glUniform1i      (GetShaderVariableLocation(shaderProgram, name), location);
}

int Renderer::GetShaderVariableLocation(const ShaderProgram* shaderProgram, const char* name) const
{
    return glGetUniformLocation(shaderProgram->GetId(), name);
}

void Renderer::BindRenderTexture(const RenderTexture* renderTexture) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->GetPipelineData().FBO);
}

void Renderer::UnbindRenderTexture() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ClearBindedRenderTexture(const RGBA& color, const bool& onlyStencil) const
{
    uint clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    if (renderObjectsToStencil) clearFlags |= GL_STENCIL_BUFFER_BIT;
    if (onlyStencil)            clearFlags  = GL_STENCIL_BUFFER_BIT;
    
    glClearColor(color.r, color.g, color.b, color.a);
    glClearStencil(0);
    glClear(clearFlags);
}

bool Renderer::PrepareDraw(const SubMesh* subMesh, const Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId, const bool& instanced) const
{
    // Get the sub-mesh's shader program, material and vertex array object.
    const ShaderProgram* shaderProgram = instanced ? subMesh->GetInstancedShaderProgram() : subMesh->GetShaderProgram();
    const Material*      material      = subMesh->GetMaterial();
    const uint           vertexArrayId = subMesh->GetPipelineData().VAO;

    // Make sure the sub-mesh has been loaded properly.
    if (!subMesh->WasSentToPipeline() || !vertexArrayId || !lightManager ||
        !shaderProgram || !shaderProgram->WasSentToPipeline())
    {
        return false;
    }

    // Use the mesh's shader program and run its shader script.
    UseShaderProgram(shaderProgram);
    if (MeshShaderScript* shaderScript = dynamic_cast<MeshShaderScript*>(shaderProgram->GetShaderScript()))
    {
        shaderScript->subMesh      = subMesh;
        shaderScript->material     = material;
        shaderScript->modelMat     = &modelMat;
        shaderScript->camera       = camera;
        shaderScript->lightManager = lightManager;
        shaderScript->SetShaderVariables();
    }

    // Draw the object ID onto the stencil buffer if it is enabled and if object ID is non-zero.
    if (renderObjectsToStencil && objectId > 0)
    {
        glStencilFunc(GL_ALWAYS, (GLint)objectId, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
    }
    
    glBindVertexArray(vertexArrayId);
    return true;
}

void Renderer::DrawSubMesh(const SubMesh* subMesh, const Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId) const
{
    if (!PrepareDraw(subMesh, modelMat, camera, lightManager, objectId)) return;

    if (subMesh->GetType() == SubMeshType::Animated)
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ((const AnimatedSubMesh*)subMesh)->GetBoneMatricesBuffer()); // TODO: Move this to a shader script.
        glCullFace(GL_FRONT); // TODO: Find why normals are reversed here and not on the Animation branch.
    }

    // Draw the sub-mesh.
    glDrawElements(GL_TRIANGLES, (int)subMesh->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    if (subMesh->GetType() == SubMeshType::Animated)
    {
        glCullFace(GL_BACK);
    }
}

void Renderer::DrawInstanced(const SubMesh* subMesh, const int& instanceCount, const Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId) const
{
    if (instanceCount < 1 || !subMesh->GetPipelineData().IsInstanced())        return;
    if (!PrepareDraw(subMesh, modelMat, camera, lightManager, objectId, true)) return;

    if (subMesh->GetType() == SubMeshType::Animated)
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ((const AnimatedSubMesh*)subMesh)->GetBoneMatricesBuffer()); // TODO: Move this to a shader script.
        glCullFace(GL_FRONT); // TODO: Find why normals are reversed here and not on the Animation branch.
    }
    
    // Draw the sub-mesh.
    glBindBuffer(GL_ARRAY_BUFFER, subMesh->GetPipelineData().instanceBO);
    glDrawElementsInstanced(GL_TRIANGLES, (int)subMesh->GetVertexCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (subMesh->GetType() == SubMeshType::Animated)
    {
        glCullFace(GL_BACK);
    }
}

void Renderer::DrawCubemap(const SubMesh* subMesh, const Cubemap* cubemap, const Mat4& modelMat, const Camera* camera, const LightManager* lightManager, const size_t& objectId) const
{
    if (!PrepareDraw(subMesh, modelMat, camera, lightManager, objectId) || !cubemap || !cubemap->WasSentToPipeline()) return;

    // Draw the mesh.
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
    glDrawElements(GL_TRIANGLES, (int)subMesh->GetVertexCount(), GL_UNSIGNED_INT, 0);
    glCullFace(GL_BACK);
    glBindVertexArray(0);
}

void Renderer::DrawScreenTexture(const Texture* texture, const ShaderProgram* shaderProgram) const
{
    // TODO: This probably should get cleaned somehow.
    static ShaderProgram* defaultShader = nullptr;
    if (!shaderProgram && !defaultShader)
    {
        defaultShader = Engine::Get()->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Screen/screenShader.prog");
        return;
    }

    const ShaderProgram* shader = shaderProgram ? shaderProgram : defaultShader;
    UseShaderProgram(shader);
    if (ScreenShaderScript* shaderScript = dynamic_cast<ScreenShaderScript*>(shader->GetShaderScript()))
    {
        shaderScript->textureID  = texture->GetId();
        shaderScript->rectPos    = Vector2(0, 0);
        shaderScript->rectSize   = texture->GetSize();
        shaderScript->screenSize = Engine::Get()->GetWindow()->GetSize();
        shaderScript->SetShaderVariables();
    }

    if (renderObjectsToStencil)
    {
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    }
    
    glBindVertexArray(screenRect.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetId());
    glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE);
}

void Renderer::DrawScreenTexture(const RenderTexture* texture, const ShaderProgram* shaderProgram) const
{
    // TODO: This probably should get cleaned somehow.
    static ShaderProgram* defaultShader = nullptr;
    if (!shaderProgram && !defaultShader)
    {
        defaultShader = Engine::Get()->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Screen/screenShader.prog");
        return;
    }
    
    const ShaderProgram* shader = shaderProgram ? shaderProgram : defaultShader;
    UseShaderProgram(shader);
    if (ScreenShaderScript* shaderScript = dynamic_cast<ScreenShaderScript*>(shader->GetShaderScript()))
    {
        shaderScript->textureID  = texture->GetPipelineData().ID;
        shaderScript->rectPos    = Vector2(0, 0);
        shaderScript->rectSize   = texture->GetSize();
        shaderScript->screenSize = Engine::Get()->GetWindow()->GetSize();
        shaderScript->SetShaderVariables();
    }

    if (renderObjectsToStencil)
    {
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    }
    
    glBindVertexArray(screenRect.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetPipelineData().ID);
    glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE);
}

void Renderer::DrawTexture2D(const Texture* texture, const Vector2& size, const Vector2& pos, const Vector2& texSize, const size_t& id) const
{
    // TODO: This probably should get cleaned somehow.
    static ShaderProgram* shader = nullptr;
    if (!shader)
    {
        shader = Engine::Get()->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Screen/screenShader.prog");
        return;
    }

    UseShaderProgram(shader);
    SetShaderVariable(shader, "scale",  size);
    SetShaderVariable(shader, "offset", pos);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(uiRect.VAO);
    
    //pos = pos / Engine::Get()->GetWindow()->GetSize();
    const float posX = pos.x - texSize.x * 0.5f;
    const float posY = pos.y - texSize.y * 0.5f;

    // Define vertex positions and uvs.
    const float vertices[6][4] = {
        { posX,	            posY + texSize.y, 0.0f, 1.0f },
        { posX,             posY,             0.0f, 0.0f },
        { posX + texSize.x, posY,             1.0f, 0.0f },
    
        { posX,             posY + texSize.y, 0.0f, 1.0f },
        { posX + texSize.x, posY,             1.0f, 0.0f },
        { posX + texSize.x, posY + texSize.y, 1.0f, 1.0f }
    };
    
    // Update contents of VBO memory.
    glBindBuffer(GL_ARRAY_BUFFER, uiRect.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the object ID onto the stencil buffer if it is enabled and if object ID is non-zero.
    if (renderObjectsToStencil)
    {
        glStencilFunc(GL_ALWAYS, (GLint)id, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
    }

    // Render the given texture over the quad.
    glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, texture->GetId());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE);
}

void Renderer::DrawLine(const Vector2& start, const Vector2& end, const RGB& startColor, const RGB& endColor) const
{
    const float line[10] = {
        start.x, start.y, startColor.r, startColor.g, startColor.b,
        end  .x, end  .y, endColor  .r, endColor  .g, endColor  .b
    };
    static uint vao = 0;
    static uint vbo = 0;
    static ShaderProgram* shader = nullptr;
    if (!vao)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers     (1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        VertexShader*   vertex   = new VertexShader  ("Resources/Shaders/Screen/lineShader.vert");
        FragmentShader* fragment = new FragmentShader("Resources/Shaders/Screen/lineShader.frag");
                        shader   = new ShaderProgram ("Resources/Shaders/Screen/lineShader.prog");
        vertex  ->Load(); vertex  ->SendToPipeline();
        fragment->Load(); fragment->SendToPipeline();
        shader  ->AttachShaders(2, vertex, fragment);
        shader  ->Load(); shader  ->SendToPipeline();
    }
    else
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line), line);
    }
    UseShaderProgram(shader);
    glDrawArrays(GL_LINES, 0, 4);
}

void Renderer::DrawLine(const Vector3& start, const Vector3& end, const RGB& startColor, const RGB& endColor, const Camera* camera) const
{
    const Mat4    viewProjMat = camera->GetViewMat() * camera->GetProjectionMat();
    const Vector2 screenPos1  = (Vector4(start, 1) * viewProjMat).ToVector3(true).ToVector2();
    const Vector2 screenPos2  = (Vector4(end,   1) * viewProjMat).ToVector3(true).ToVector2();
    DrawLine(screenPos1, screenPos2, startColor, endColor);
}

void Renderer::DrawText(const std::string& text, const Font* font, const Vector2& pos, const Vector2& scale, const RGBA& color, const size_t& id) const
{
    // TODO: This probably should get cleaned somehow.
    static ShaderProgram* shader = nullptr;
    if (!shader)
    {
        shader = Engine::Get()->GetResourceManager()->Get<ShaderProgram>("Resources/Shaders/Screen/textShader.prog"); 
        return;
    }
    
    UseShaderProgram (shader);
    SetShaderVariable(shader, "textColor", color);
    SetShaderVariable(shader, "scale",  Vector2(1));
    SetShaderVariable(shader, "offset", Vector2(0));
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(uiRect.VAO);
    glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);

    const Vector2 realScale = scale * 0.01f;
    Vector2 realPos = pos * 0.01f;

    for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
    {
        const Character& ch = font->GetCharacter(*c);

        const float x = realPos.x + ch.GetBearing().x * realScale.x;
        const float y = realPos.y - (ch.GetSize().y - ch.GetBearing().y)* realScale.y;

        const float w = ch.GetSize().x * realScale.x;
        const float h = ch.GetSize().y * realScale.y;
        
        // Define vertex positions and uvs for each character.
        const float vertices[6][4] = {
            { x,   y+h, 0.0f, 0.0f },
            { x,   y,   0.0f, 1.0f },
            { x+w, y,   1.0f, 1.0f },

            { x,   y+h, 0.0f, 0.0f },
            { x+w, y,   1.0f, 1.0f },
            { x+w, y+h, 1.0f, 0.0f }
        };
        
        // Update contents of VBO memory.
        glBindBuffer(GL_ARRAY_BUFFER, uiRect.VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Draw the object ID onto the stencil buffer if it is enabled and if object ID is non-zero.
        if (renderObjectsToStencil)
        {
            glStencilFunc(GL_ALWAYS, (GLint)id, 0xFF);
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
            glStencilMask(0xFF);
        }
        
        // Render the glyph's texture over the quad.
        glBindTexture(GL_TEXTURE_2D, ch.GetTextureID());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Advance cursor for the next glyph (advance is a number of 1/64 pixels).
        realPos.x += (float)(ch.GetAdvance() >> 6) * realScale.x;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE);
}

void Renderer::RenderObjectsToStencil(const bool& enable)
{
    if (enable) glEnable (GL_STENCIL_TEST);
    else        glDisable(GL_STENCIL_TEST);
    renderObjectsToStencil = enable;
}

void Renderer::RenderWireframe(const bool& enable) const
{
    if (enable) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

size_t Renderer::PickObjectFromViewport(const Vector2& pickPos, const RenderTexture* viewportTexture, const Vector2& viewportSize) const
{
    if (!renderObjectsToStencil) return 0;
    
    const Vector2 textureSize = viewportTexture->GetSize();
    const Vector2 sizeRatio   = viewportSize / textureSize;
    const Vector2 uvOffset    = (Vector2(1) - sizeRatio) * 0.5f;
    const Vector2 pxOffset    = uvOffset * textureSize;
          Vector2 readPos     = pxOffset + pickPos; readPos.y = textureSize.y - readPos.y;

    if (!((0 < readPos.x && readPos.x < textureSize.x) || (0 < readPos.y && readPos.y < textureSize.y))) {
        return 0;
    }
        
    GLint id = 0;
    BindRenderTexture(viewportTexture);
    glReadPixels((int)readPos.x, (int)readPos.y, 1, 1, GL_STENCIL_INDEX, GL_INT, &id);
    UnbindRenderTexture();
    return id;
}

void Renderer::CompileShader(const uint& id, const ResourceType& type, const char* curFile, const char* curFunction, const long& curLine) const
{
    glCompileShader(id);

    // Check for shader compile errors.
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        std::string shaderName;
        switch (type)
        {
        case ResourceType::VertexShader:   shaderName = "Vertex";   break;
        case ResourceType::FragmentShader: shaderName = "Fragment"; break;
        default: break;
        }
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        Assertion(success, shaderName + std::string(" shader compilation failed:\n") + infoLog, curFile, curFunction, curLine);
    }
}




// ---------- SendResourceToPipeline ---------- //

template <typename T> void Renderer::SendResourceToPipeline(T* resource) const
{
    DebugLogError("The given resource type cannot be sent to GPU memory.");
}

template <> void Renderer::SendResourceToPipeline(VertexShader* resource) const
{
    // Create and compile shader.
    const std::string& shaderSourceStr = resource->GetSourceCode();
    const char*        shaderSource    = shaderSourceStr.c_str();
    const uint id = glCreateShader(GL_VERTEX_SHADER);
    resource->SetId(id);
    glShaderSource(id, 1, &shaderSource, 0);
    CompileShader(id, ResourceType::VertexShader, __FILENAME__, __FUNCTION__, __LINE__);
}


template <> void Renderer::SendResourceToPipeline(FragmentShader* resource) const
{
    // Create and compile shader.
    const std::string& shaderSourceStr = resource->GetSourceCode();
    const char*        shaderSource    = shaderSourceStr.c_str();
    const uint id = glCreateShader(GL_FRAGMENT_SHADER);
    resource->SetId(id);
    glShaderSource(id, 1, &shaderSource, 0);
    CompileShader(id, ResourceType::FragmentShader, __FILENAME__, __FUNCTION__, __LINE__);
}


template <> void Renderer::SendResourceToPipeline(ShaderProgram* resource) const
{
    // Create the program, attach all shaders and link.
    const uint id = glCreateProgram();
    resource->SetId(id);
    for (Resource* shader : resource->GetAttachedShaders())
    {
        switch (shader->GetType())
        {
        case ResourceType::VertexShader:
            glAttachShader(id, ((VertexShader*)shader)->GetId());
            break;
        case ResourceType::FragmentShader:
            glAttachShader(id, ((FragmentShader*)shader)->GetId());
            break;
        default:
            break;
        }
    }
    glLinkProgram(id);

    // Check for linking errors.
    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        Assert(success, std::string("Shader program linking failed:\n") + infoLog);
    }
}

template <> void Renderer::SendResourceToPipeline(Texture* resource) const
{
    uint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    resource->SetId(id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLfloat largest;
    glGetFloatv    (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,           &largest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);

    GLint format;
    switch (resource->GetChannels())
    {
    case 1:  format = GL_RED;  break;
    case 3:  format = GL_RGB;  break;
    default: format = GL_RGBA; break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, resource->GetWidth(), resource->GetHeight(), 0, format, GL_UNSIGNED_BYTE, resource->GetData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

template <> void Renderer::SendResourceToPipeline(RenderTexture* resource) const
{
    RenderTexturePipelineData pipelineData;
    
    // Create Frame Buffer Object.
    glGenFramebuffers(1, &pipelineData.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, pipelineData.FBO);

    // Create Framebuffer Texture.
    glGenTextures(1, &pipelineData.ID);
    glBindTexture(GL_TEXTURE_2D, pipelineData.ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resource->GetWidth(), resource->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pipelineData.ID, 0);

    // Create Render Buffer Object.
    glGenRenderbuffers(1, &pipelineData.RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, pipelineData.RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resource->GetWidth(), resource->GetHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pipelineData.RBO);

    // Error checking framebuffer.
    const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        DebugLogError("Unable to create rendertexture framebuffer (error code: " + std::to_string(fboStatus) + ")");
        glDeleteRenderbuffers(1, &pipelineData.RBO);
        glDeleteTextures(1, &pipelineData.ID);
        glDeleteFramebuffers(1, &pipelineData.FBO);
        return;
    }
    resource->SetPipelineData(pipelineData);
}

template <> void Renderer::SendResourceToPipeline(StaticSubMesh* resource) const
{
    SubMeshPipelineData pipelineData = resource->GetPipelineData();

    if (pipelineData.IsEmpty())
    {
        // Get the vertices and indices of the mesh.
        const std::vector<TangentVertex>& vertices = resource->GetVertices();
        const std::vector<uint         >& indices  = resource->GetIndices ();
        if (vertices.empty()) return;

        // Create and bind the Vertex Array Object.
        glGenVertexArrays(1, &pipelineData.VAO);
        glBindVertexArray(pipelineData.VAO);

        // Create, bind and set the vertex buffer.
        glGenBuffers(1, &pipelineData.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, pipelineData.VBO);
        glBufferData(GL_ARRAY_BUFFER, (int)(vertices.size() * sizeof(TangentVertex)), vertices.data(), GL_STATIC_DRAW);

        // Create, bind and set the index buffer.
        glGenBuffers(1, &pipelineData.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipelineData.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

        // Set the position attribute pointer.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)0);

        // Set the uv attribute pointer.
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)(3 * sizeof(float)));

        // Set the normal attribute pointer.
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)(5 * sizeof(float)));

        // Set the tangent attribute pointer.
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)(8 * sizeof(float)));

        // Set the bitTangent attribute pointer.
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TangentVertex), (void*)(11 * sizeof(float)));

        resource->SetPipelineData(pipelineData);
    }
    else if (pipelineData.IsInstanced())
    {
        glBindVertexArray(pipelineData.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, pipelineData.instanceBO);
    
        // Set the instance matrices attribute pointer.
        glEnableVertexAttribArray(10);
        glEnableVertexAttribArray(11);
        glEnableVertexAttribArray(12);
        glEnableVertexAttribArray(13);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)0);
        glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(1 * sizeof(Vector4)));
        glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(2 * sizeof(Vector4)));
        glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(3 * sizeof(Vector4)));
        glVertexAttribDivisor(10, 1);
        glVertexAttribDivisor(11, 1);
        glVertexAttribDivisor(12, 1);
        glVertexAttribDivisor(13, 1);
    }
    glBindVertexArray(0);
}

template <> void Renderer::SendResourceToPipeline(AnimatedSubMesh* resource) const
{
    SubMeshPipelineData pipelineData = resource->GetPipelineData();

    if (pipelineData.IsEmpty())
    {
        // Get the vertices and indices of the mesh.
        const std::vector<AnimatedVertex>& vertices = resource->GetVertices();
        const std::vector<uint>& indices = resource->GetIndices();
        if (vertices.empty()) return;
	
        // Create and bind the Vertex Array Object.
        glGenVertexArrays(1, &pipelineData.VAO);
        glBindVertexArray(pipelineData.VAO);

        // Create, bind and set the vertex buffer.
        glGenBuffers(1, &pipelineData.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, pipelineData.VBO);
        glBufferData(GL_ARRAY_BUFFER, (int)(vertices.size() * sizeof(AnimatedVertex)), vertices.data(), GL_STATIC_DRAW);

        // Create, bind and set the index buffer.
        glGenBuffers(1, &pipelineData.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipelineData.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

        // Set the position attribute pointer.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Set the uv attribute pointer.
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Set the normal attribute pointer.
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Set the tangent attribute pointer.
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Set the bitangent attribute pointer.
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);

        // Set the boneIds attribute pointer
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(AnimatedVertex), (void*)(14 * sizeof(float)));
        glEnableVertexAttribArray(5);
	
        // Set the boneWeights attribute pointer.
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(14 * sizeof(float) + 4 * sizeof(int)));
        glEnableVertexAttribArray(6);

        resource->SetPipelineData(pipelineData);
    }
    else if (pipelineData.IsInstanced())
    {
        glBindVertexArray(pipelineData.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, pipelineData.instanceBO);
    
        // Set the instance matrices attribute pointer.
        glEnableVertexAttribArray(10);
        glEnableVertexAttribArray(11);
        glEnableVertexAttribArray(12);
        glEnableVertexAttribArray(13);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)0);
        glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(1 * sizeof(Vector4)));
        glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(2 * sizeof(Vector4)));
        glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void*)(3 * sizeof(Vector4)));
        glVertexAttribDivisor(10, 1);
        glVertexAttribDivisor(11, 1);
        glVertexAttribDivisor(12, 1);
        glVertexAttribDivisor(13, 1);
    }
    glBindVertexArray(0);
}

template <> void Renderer::SendResourceToPipeline(Anim::Skeleton* resource) const
{
	if (resource->boneMatrices.empty()) return;

	uint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (int)(sizeof(Mat4) * resource->boneMatrices.size()), resource->boneMatrices.data(), GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	resource->boneMatricesBuffer = buffer;
}

template <> void Renderer::SendResourceToPipeline(Cubemap* resource) const
{
    glGenTextures(1, &resource->id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, resource->id);

	ResourceManager* resourceManager = Engine::Get()->GetResourceManager();
	for(int i = 0; i < 6; ++i)
	{
		resourceManager->LoadFile(resource->sides[i], false);
		Texture* texture = resourceManager->Get<Texture>(resource->sides[i]);
		texture->SetFlippedVertically(false);
		
		uint id = 0;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD,    0); // Disable mipmaps.
	    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD,    0); // Disable mipmaps.
	    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  0); // Disable mipmaps.
	
		GLfloat anisotropy;
		GLint   format;
		glGetFloatv    (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,           &anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	    
		switch (texture->GetChannels())
		{
			case 1:  format = GL_RED;  break;
			case 3:  format = GL_RGB;  break;
			default: format = GL_RGBA; break;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture->GetWidth(), texture->GetHeight(), 0, format, GL_UNSIGNED_BYTE, texture->GetData());
	    glGenerateMipmap(GL_TEXTURE_2D); // This generates a GL error on AMD for whatever reason.
		texture->SetId(id);
	}

    // TODO: Find a way to enable mipmaps on cube maps that works on any GPU.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD,    0); // Disable mipmaps.
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD,    0); // Disable mipmaps.
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL,  0); // Disable mipmaps.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // This generates a GL error on AMD for whatever reason.
}

template <> void Renderer::SendResourceToPipeline(Font* resource) const
{
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (uchar c = 0; c < 128; c++)
    {
        // Load character glyph.
        const FT_Face face = resource->GetFace(c);
        
        // Generate texture.
        uint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // Set texture options.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD,    0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD,    0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  0);
    
        GLfloat largest;
        glGetFloatv    (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,           &largest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);

        // Set image data and generate mipmaps.
        const int width  = (int)face->glyph->bitmap.width;
        const int height = (int)face->glyph->bitmap.rows;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // glGenerateMipmap(GL_TEXTURE_2D); // This generates a GL error on AMD for whatever reason.
        
        // Store character for later use.
        const Character character = Character(
            texture,
            Vector2((float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows),
            Vector2((float)face->glyph->bitmap_left,  (float)face->glyph->bitmap_top ),
            (uint)face->glyph->advance.x
        );
        resource->AddCharacter(c, character);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

// ---------- DeleteResource ---------- //

template <typename T> void Renderer::DeleteResource(T* resource) const
{
    DebugLogError("The given resource type cannot be deleted from GPU memory.");
}

template <> void Renderer::DeleteResource(VertexShader* resource) const
{
    const uint id = resource->GetId();
    glDeleteShader(id);
}

template <> void Renderer::DeleteResource(FragmentShader* resource) const
{
    const uint id = resource->GetId();
    glDeleteShader(id);
}

template <> void Renderer::DeleteResource(ShaderProgram* resource) const
{
    const uint id = resource->GetId();
    glDeleteProgram(id);
}

template <> void Renderer::DeleteResource(Texture* resource) const
{
    const uint id = resource->GetId();
    glDeleteTextures(1, &id);
}

template <> void Renderer::DeleteResource(RenderTexture* resource) const
{
    const auto [FBO, RBO, ID] = resource->GetPipelineData();
    if (RBO) { glDeleteRenderbuffers(1, &RBO); }
    if (ID ) { glDeleteTextures     (1, &ID ); }
    if (FBO) { glDeleteFramebuffers (1, &FBO); }
}

template <> void Renderer::DeleteResource(SubMesh* resource) const
{
    const auto [VBO, EBO, VAO, instanceVBO] = resource->GetPipelineData();
    if (EBO)         { glDeleteBuffers     (1, &EBO); }
    if (VBO)         { glDeleteBuffers     (1, &VBO); }
    if (VAO)         { glDeleteVertexArrays(1, &VAO); }
    if (instanceVBO) { glDeleteBuffers     (1, &instanceVBO); }
}

template <> void Renderer::DeleteResource(Anim::Skeleton* resource) const
{
    glDeleteBuffers(1, &resource->boneMatricesBuffer);
}

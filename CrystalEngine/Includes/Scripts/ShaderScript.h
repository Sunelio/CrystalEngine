#pragma once
#include "Maths/Vector2.h"
#include "Utils/Typedefs.h"

#include <Refureku/Refureku.h>
#include "Refureku/Generated/ShaderScript.rfkh.h"

namespace Resources
{
    class SubMesh;
    class ShaderProgram;
    class Material;
}
namespace Maths
{ 
	template<int R, int C> class Matrix;
    typedef Matrix<4, 4> Mat4; 
}
namespace Render
{
    class PostProcessor;
    class Camera;
    class LightManager;
    class Renderer;
}
namespace Core   { class Engine; }

namespace Scripts NAMESPACE()
{
    // - ShaderScript: Generic class from which more specific shader scripts inherit - //
    struct STRUCT() ShaderScript
    {
        // -- Attributes -- //
        // * All attributes are set before the script is run. * //
        FIELD() const Resources::ShaderProgram* shader       = nullptr;
        FIELD() const Render::Renderer*         renderer     = nullptr;
        FIELD() const Core::Engine*             engine       = nullptr;

        // -- Methods -- //
        ShaderScript();
        virtual ~ShaderScript()                      = default;
        ShaderScript(const ShaderScript&)            = delete;
        ShaderScript(ShaderScript&&)                 = delete;
        ShaderScript& operator=(const ShaderScript&) = delete;
        ShaderScript& operator=(ShaderScript&&)      = delete;

        virtual void SetShaderVariables() {} // Overriden by children to set their shader's variable values.

        Scripts_ShaderScript_GENERATED
    };
    
    // - MeshShaderScript: Parent class of all shader scripts that are run on meshes - //
    struct STRUCT() MeshShaderScript : ShaderScript
    {
        // -- Attributes -- //
        // * All attributes are set before the script is run. * //
        FIELD() const Resources::SubMesh*   subMesh      = nullptr;
        FIELD() const Resources::Material*  material     = nullptr;
        FIELD() const Maths::Mat4*          modelMat     = nullptr;
        FIELD() const Render::Camera*       camera       = nullptr;
        FIELD() const Render::LightManager* lightManager = nullptr;

        Scripts_MeshShaderScript_GENERATED
    };

    // - ScreenShaderScript: Parent class of all shader scripts that are run on UI or Post-Processing textures - //
    struct STRUCT() ScreenShaderScript : ShaderScript
    {
        // -- Attributes -- //
        // * All attributes are set before the script is run. * //
        FIELD() uint textureID = 0;
        FIELD() Maths::Vector2 rectPos;
        FIELD() Maths::Vector2 rectSize;
        FIELD() Maths::Vector2 screenSize;
        FIELD() const Render::PostProcessor* postProcessor = nullptr;

        METHOD() ScreenShaderScript();

        Scripts_ScreenShaderScript_GENERATED
    };
}
File_ShaderScript_GENERATED


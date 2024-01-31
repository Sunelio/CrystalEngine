#pragma once
#include "Maths/Color.h"
#include "Scripts/ObjectScript.h"
#include "Scripts/ShaderScript.h"
#include <Refureku/Generated/DefaultScripts.rfkh.h>

namespace Scripts NAMESPACE()
{
    struct STRUCT() RotateObject : ObjectScript
    {
        RotateObject();
        METHOD() void Update() override;

        Scripts_RotateObject_GENERATED
    };
    
    struct STRUCT() DefaultMeshShaderScript : MeshShaderScript
    {
        bool unlit         = false;
        bool usePlainColor = false;
        Maths::RGBA plainColor;
        METHOD() void SetShaderVariables() override;

    private:
        void SendMaterialToShader() const;
        void SendLightsToShader  () const;

        Scripts_DefaultMeshShaderScript_GENERATED
    };

    struct STRUCT() GridMeshShaderScript : MeshShaderScript
    {
        METHOD() void SetShaderVariables() override;

        Scripts_GridMeshShaderScript_GENERATED
    };

    struct STRUCT() SkyboxMeshShaderScript : MeshShaderScript
    {
        METHOD() void SetShaderVariables() override;

        Scripts_SkyboxMeshShaderScript_GENERATED
    };
    
    struct STRUCT() DefaultScreenShaderScript : ScreenShaderScript
    {
        METHOD() void SetShaderVariables() override;

        Scripts_DefaultScreenShaderScript_GENERATED
    };
}

File_DefaultScripts_GENERATED

#include "Scripts/DefaultScripts.h"
#include "Core/TimeManager.h"
#include "Maths/MathConstants.h"
#include "Render/Renderer.h"
#include "Render/Camera.h"
#include "Maths/Matrix.h"
#include "Render/LightManager.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Refureku/Generated/ObjectScript.rfks.h"
#include "Refureku/Generated/DefaultScripts.rfks.h"
#include "Refureku/Generated/ObjectScript.rfks.h"

using namespace Scripts;

ObjectScript::ObjectScript() { archId = staticGetArchetype().getId(); }
RotateObject::RotateObject() { archId = staticGetArchetype().getId(); }

void RotateObject::Update()
{
    transform->RotateEuler({ 0, time->DeltaTime() * PI/8, 0 });
}

void DefaultMeshShaderScript::SetShaderVariables()
{
    // Send matrices to shader.
    renderer->SetShaderVariable(shader, "mvpMatrix", *modelMat * camera->GetViewMat() * camera->GetProjectionMat());
    renderer->SetShaderVariable(shader, "modelMat",  *modelMat);

    // Send the camera position to shader.
    renderer->SetShaderVariable(shader, "viewPos", camera->transform->GetPosition());

    // Set the rendering parameters variables.
    renderer->SetShaderVariable(shader, "unlit",         unlit);
    renderer->SetShaderVariable(shader, "usePlainColor", usePlainColor);
    renderer->SetShaderVariable(shader, "plainColor",    plainColor);

    // Send material and light data to shader.
    if (material) SendMaterialToShader();
    SendLightsToShader();
}

void DefaultMeshShaderScript::SendMaterialToShader() const
{
    // Send material parameters to shader.
    renderer->SetShaderVariable(shader, "material.ambient",      material->ambient     );
    renderer->SetShaderVariable(shader, "material.diffuse",      material->diffuse     );
    renderer->SetShaderVariable(shader, "material.specular",     material->specular    );
    renderer->SetShaderVariable(shader, "material.emission",     material->emission    );
    renderer->SetShaderVariable(shader, "material.shininess",    material->shininess   );
    renderer->SetShaderVariable(shader, "material.transparency", material->transparency);

    // Cull back faces of non-transparent models.
    /*
    if (material->transparency >= 1 && material->alphaMap == nullptr)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    */

    // Send ambient texture to shader.
    if (material->ambientTexture != nullptr && material->ambientTexture->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.ambientTex.texture", 0, material->ambientTexture->GetId());
        renderer->SetShaderVariable(shader, "material.ambientTex.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.ambientTex.assigned", false);
    }

    // Send diffuse texture to shader.
    if (material->diffuseTexture != nullptr && material->diffuseTexture->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.diffuseTex.texture", 1, material->diffuseTexture->GetId());
        renderer->SetShaderVariable(shader, "material.diffuseTex.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.diffuseTex.assigned", false);
    }

    // Send specular texture to shader.
    if (material->specularTexture != nullptr && material->specularTexture->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.specularTex.texture", 2, material->specularTexture->GetId());
        renderer->SetShaderVariable(shader, "material.specularTex.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.specularTex.assigned", false);
    }

    // Send emission texture to shader.
    if (material->emissionTexture != nullptr && material->emissionTexture->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.emissionTex.texture", 3, material->emissionTexture->GetId());
        renderer->SetShaderVariable(shader, "material.emissionTex.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.emissionTex.assigned", false);
    }

    // Send shininess map to shader.
    if (material->shininessMap != nullptr && material->shininessMap->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.shininessMap.texture", 4, material->shininessMap->GetId());
        renderer->SetShaderVariable(shader, "material.shininessMap.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.shininessMap.assigned", false);
    }

    // Send alpha map to shader.
    if (material->alphaMap != nullptr && material->alphaMap->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.alphaMap.texture", 5, material->alphaMap->GetId());
        renderer->SetShaderVariable(shader, "material.alphaMap.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.alphaMap.assigned", false);
    }

    // Send normal map to shader.
    if (material->normalMap != nullptr && material->normalMap->WasSentToPipeline()) {
        renderer->SetShaderSampler (shader, "material.normalMap.texture", 6, material->normalMap->GetId());
        renderer->SetShaderVariable(shader, "material.normalMap.assigned", true);
    }
    else {
        renderer->SetShaderVariable(shader, "material.normalMap.assigned", false);
    }
}

void DefaultMeshShaderScript::SendLightsToShader() const
{
    Render::Light*const* lights = lightManager->GetLights();
    const int lightMemberLocs[11] = 
    {
        renderer->GetShaderVariableLocation(shader, "lights[0].assigned"),
        renderer->GetShaderVariableLocation(shader, "lights[0].ambient"),
        renderer->GetShaderVariableLocation(shader, "lights[0].diffuse"),
        renderer->GetShaderVariableLocation(shader, "lights[0].specular"),
        renderer->GetShaderVariableLocation(shader, "lights[0].constant"),
        renderer->GetShaderVariableLocation(shader, "lights[0].linear"),
        renderer->GetShaderVariableLocation(shader, "lights[0].quadratic"),
        renderer->GetShaderVariableLocation(shader, "lights[0].outerCutoff"),
        renderer->GetShaderVariableLocation(shader, "lights[0].innerCutoff"),
        renderer->GetShaderVariableLocation(shader, "lights[0].position"),
        renderer->GetShaderVariableLocation(shader, "lights[0].direction"),
    };
    for (uint i = 0; i < Render::LightManager::MAX_LIGHTS; i++)
    {
        const int curLightLoc = i*11;
        if (lights[i] && lights[i]->transform)
        {
            const Maths::Vector3 position  = lights[i]->transform->GetWorldPosition();
            const Maths::Vector3 direction = lights[i]->transform->Forward();

            // Set the current light as assigned and set all its parameters.
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[0 ], true);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[1 ], lights[i]->ambient);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[2 ], lights[i]->diffuse);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[3 ], lights[i]->specular);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[4 ], lights[i]->constant);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[5 ], lights[i]->linear);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[6 ], lights[i]->quadratic);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[7 ], lights[i]->outerCutoff);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[8 ], Maths::clampAbove(lights[i]->innerCutoff, lights[i]->outerCutoff));
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[9 ], position);
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[10], direction);
        }
        else
        {
            // Set the current light as unassigned.
            renderer->SetShaderVariable(curLightLoc + lightMemberLocs[0], false);
        }
    }
}

void SkyboxMeshShaderScript::SetShaderVariables()
{
    renderer->SetShaderVariable(shader, "mvpMatrix", *modelMat * camera->transform->GetRotation().GetConjugate().ToMatrix() * camera->GetProjectionMat());
}

void DefaultScreenShaderScript::SetShaderVariables()
{
    renderer->SetShaderVariable(shader, "scale",  rectSize / screenSize);
    renderer->SetShaderVariable(shader, "offset", rectPos);
}

void GridMeshShaderScript::SetShaderVariables()
{
    renderer->SetShaderVariable(shader, "mvpMatrix", *modelMat * camera->GetViewMat() * camera->GetProjectionMat());
    renderer->SetShaderVariable(shader, "modelMat",  *modelMat);
}

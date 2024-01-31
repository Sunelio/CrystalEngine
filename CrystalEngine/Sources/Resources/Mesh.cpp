#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"

#include <algorithm>
#include <LightJson/JsonElement.h>

#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Resources/ResourceManager.h"

using namespace Resources;

Mesh::Mesh(const std::string& _name)
{
    name = _name;
    type = ResourceType::Mesh;
}

Mesh::~Mesh()
{
    for (size_t i = 0; i < subMeshes.size(); i++)
        delete subMeshes[i];
    delete skeleton;
}

void Mesh::Load()
{
    if (AreSubMeshesLoaded())
        loaded = true;
}

void Mesh::SendToPipeline()
{
    if (subMeshes.empty() || !IsLoaded() || WasSentToPipeline())
        return;
    
    if (skeleton) skeleton->SendToPipeline();
    
    for (size_t i = 0; i < subMeshes.size(); i++)
    {
        switch (subMeshes[i]->GetType())
        {
        case SubMeshType::Static:
            ((StaticSubMesh*)subMeshes[i])->SendVerticesToPipeline(vertexCount);
            break;
        case SubMeshType::Animated:
            ((AnimatedSubMesh*)subMeshes[i])->SendVerticesToPipeline(vertexCount);
            ((AnimatedSubMesh*)subMeshes[i])->SetBoneMatricesBuffer(skeleton->GetBoneMatricesBuffer());
            break;
        default:
            break;
        }
    }
    
    if (WereSubMeshesSentToPipeline())
	    sentToPipeline = true;
}

bool Mesh::AreSubMeshesLoaded() const
{
    if (subMeshes.empty())
        return false;
    return std::all_of(subMeshes.begin(), subMeshes.end(), [](const SubMesh* subMesh){ return subMesh->IsLoaded(); });
}

bool Mesh::WereSubMeshesSentToPipeline() const
{
    if (subMeshes.empty())
        return false;
    return std::all_of(subMeshes.begin(), subMeshes.end(), [](const SubMesh* subMesh){ return subMesh->WasSentToPipeline(); });
}

void Mesh::MakeInstanced()
{
    if (instanceMatBuffer || !WasSentToPipeline()) return;
    
    const Render::Renderer* renderer = Core::Engine::Get()->GetRenderer();
    instanceMatBuffer = renderer->CreateShaderBuffer();
    
    for (SubMesh* subMesh : subMeshes) {
        subMesh->MakeInstanced(instanceMatBuffer);
    }
}

# if 0
#include "Physics/RenderHelp.h"

#include "Resources/ShaderProgram.h"
#include "Resources/Mesh.h"
#include "Resources/ResourceManager.h"
#include "Render/CameraManager.h"
#include "Render/LightManager.h"
#include "Scenes/GameObject.h"
#include "Render/Renderer.h"
#include "Core/Engine.h"
#include "Resources/Material.h"
#include "Scripts/DefaultScripts.h"


using namespace Core;
using namespace Render;
using namespace Resources;
using namespace Scenes;
using namespace Maths;


#define InternalFunction static
#define InternalGlobalVariable static


SubMesh* Physic::RenderHelp::cubeMesh = nullptr;
SubMesh* Physic::RenderHelp::sphereMesh = nullptr;

namespace Physic
{
	InternalGlobalVariable bool loadData = true;

	void RenderHelp::InitRenderHelpData()
	{
		if (loadData)
		{
			Engine::Get()->GetResourceManager()->LoadFile("Resources/Meshes/Physic/Cube.obj", false);
			Engine::Get()->GetResourceManager()->LoadFile("Resources/Meshes/Physic/Sphere.obj", false);

			if (!cubeMesh)
			{
				cubeMesh = Engine::Get()->GetResourceManager()->Get<Mesh>("Box")->subMeshes[0];
				cubeMesh->SetMaterial(Engine::Get()->GetResourceManager()->Get<Material>("mat_Collider"));
			}

			if (!sphereMesh)
			{
				sphereMesh = Engine::Get()->GetResourceManager()->Get<Mesh>("Sphere")->subMeshes[0];
				sphereMesh->SetMaterial(Engine::Get()->GetResourceManager()->Get<Material>("mat_Collider"));
			}

			loadData = false;
		}
	}

	void RenderHelp::DrawCube(const Mat4& modelMat, const Renderer* render, const Camera* camera, const LightManager* lightManager)
	{
		Scripts::DefaultMeshShaderScript* meshShaderScript = (Scripts::DefaultMeshShaderScript*)cubeMesh->GetShaderProgram();
		meshShaderScript->usePlainColor = true;
		meshShaderScript->plainColor = RGB(0.0f, 1.0f, 0.0f);
		render->DrawSubMesh(cubeMesh, modelMat, camera, lightManager);
		meshShaderScript->usePlainColor = false;
	}

	void RenderHelp::DrawSphere(const Mat4& modelMat, const Renderer* render, const Camera* camera, const LightManager* lightManager)
	{
		Scripts::DefaultMeshShaderScript* meshShaderScript = (Scripts::DefaultMeshShaderScript*)cubeMesh->GetShaderProgram();
		meshShaderScript->usePlainColor = true;
		meshShaderScript->plainColor = RGB(0.0f, 1.0f, 0.0f);
		render->DrawSubMesh(sphereMesh, modelMat, camera, lightManager);
		meshShaderScript->usePlainColor = false;
	}

}
#endif
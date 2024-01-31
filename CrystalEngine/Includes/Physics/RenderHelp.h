#pragma once
# if 0
#include "Maths/Matrix.h"
#include "Render/Renderer.h"
#include "Resources/Mesh.h"
#include "Rigidbody.h"

namespace Render
{
	class Renderer;
	class Camera;
	class LightManager;
}

namespace Resources
{
	class SubMesh;
}



namespace Physic
{
	class RenderHelp
	{
	public:
		RenderHelp() = delete;
		RenderHelp(const RenderHelp&) = delete;
		RenderHelp& operator=(const RenderHelp&) = delete;

		static Resources::SubMesh* cubeMesh;
		static Resources::SubMesh* sphereMesh;

		static void InitRenderHelpData();

		static void CleanUpRenderHelpData();

		static void DrawCube(const Maths::Mat4& modelMat, const class Render::Renderer* render, const class Render::Camera* camera,
		                     const class Render::LightManager* lightManager);

		static void DrawSphere(const Maths::Mat4& modelMat, const class Render::Renderer* render, const class Render::Camera* camera,
		                       const class Render::LightManager* lightManager);

	};


}
#endif
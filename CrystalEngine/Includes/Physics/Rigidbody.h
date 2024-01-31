#pragma once
#include "Physics/Geometry3D.h"
#include <vector>

#define RIGIDBODY_TYPE_BASE		0
#define RIGIDBODY_TYPE_PARTICLE 1
#define RIGIDBODY_TYPE_SPHERE	2
#define RIGIDBODY_TYPE_BOX		3

namespace Geometry3D
{
	struct OBB;
}

namespace Resources { class ResourceManager; }


namespace Render
{
	class Renderer;
	class Camera;
	class LightManager;
}


namespace Maths
{
	class Vector3;
}

namespace Physic
{
	class RigidBody
	{
	public:
		int type;
		bool takeGravity;
		bool isTrigger;


	protected:


	public:
		inline RigidBody()
		{
			type = RIGIDBODY_TYPE_BASE;
			takeGravity = true;
			isTrigger = false;
		}
		virtual inline ~RigidBody() {}

		virtual inline void Update(float deltaTime) { }
		virtual inline void Render(const class Render::Renderer* render ,class Render::Camera* camera, class Render::LightManager* lightManager) { }

		virtual inline void ApplyForces() { }
		virtual void AddForce(const Maths::Vector3& force) {}
		virtual inline void SolveConstraints(const std::vector<Geometry3D::OBB>& constraints) { }

		inline bool HasVolume()
		{
			return type == RIGIDBODY_TYPE_SPHERE || type == RIGIDBODY_TYPE_BOX;
		}

	};

}

#pragma once
#include "RigidBody.h"
#include "Maths/Vector3.h"
#include "Geometry3D.h"

#define GRAVITY_CONSTANT Maths::Vector3(0.0f, -9.82f, 0.0f)

namespace Maths
{
	class Vector3;

}


namespace Render
{
	class Renderer;
	class CameraManager;
	class LightManager;
}




namespace Resources
{
	class SubMesh;
}

namespace Scenes { class GameObject; }



namespace Physic
{
	class RigidbodyVolume : public RigidBody
	{
	public:
		Maths::Vector3 position;
		Maths::Vector3 velocity;
#ifndef LINEAR_ONLY
		Maths::Vector3 orientation;
		Maths::Vector3 angVelocity;
#endif // !LINEAR_ONLY

		Maths::Vector3 forces; //sumForces

#ifndef LINEAR_ONLY
		Maths::Vector3 torques; //sum torques
#endif // !LINEAR_ONLY
		float mass;
		float cor; //Coefficient of restituition

#ifdef DYNAMIC_FRICTION
		float staticFriction;
		float dynamicFriction;
#else
		float friction;
#endif // DYNAMIC_FRICTION


		Geometry3D::OBB box;
		Geometry3D::Sphere sphere;



		class Scenes::GameObject* owner;

		inline RigidbodyVolume(class Scenes::GameObject* m_owner,int bodyType) :position(),
			owner(m_owner),cor(0.5f), mass(1.0f),
#ifdef DYNAMIC_FRICTION
			staticFriction(0.5f),
			dynamicFriction(0.3f)
#else
			friction(0.6f)
#endif // DYNAMIC_FRICTION
		{

			type = bodyType;
		}


		virtual ~RigidbodyVolume() { }

		virtual void Render(const class Render::Renderer* render, class Render::Camera* camera,
			class Render::LightManager* lightManager);

		virtual void Update(float deltaTime); //update position

		float InvMass();
#ifndef LINEAR_ONLY
		Maths::Mat4 InvTensor();
#endif // LINEAR_ONLY

		virtual void ApplyForces();
		virtual void AddForce(const Maths::Vector3& force);

		void SynchCollisionVolumes();

		virtual void AddLinearImpulse(const Maths::Vector3& impulse);
#ifndef LINEAR_ONLY
		virtual void AddRotationalInpulse(const Maths::Vector3& point, const Maths::Vector3& impulse);
#endif // !LINEAR_ONLY






	};

	//NOTE(o.luanda) : Collision manifold
	Geometry3D::CollisionManifold FindCollisionFeatures(Physic::RigidbodyVolume& ra, Physic::RigidbodyVolume& rb);
	void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const Geometry3D::CollisionManifold& M, int c);
}




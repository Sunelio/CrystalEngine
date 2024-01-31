#pragma once
#include "Physics/RigidBody.h"

#include "Maths/Vector3.h"
#include "Maths/Transform.h"

#include "Geometry3D.h"

namespace Maths
{
	class Vector3;
	class Transform;

}

namespace Render
{
	class Renderer;
	class CameraManager;
	class LightManager;
}

#define EULER_INTEGRATION
//#define ACCURATE_EULER_INTEGRATION


namespace Physic
{
	class Particle : public RigidBody
	{
	public:
		Maths::Vector3 position;
		Maths::Vector3 oldPosition;
		Maths::Vector3 forces;

		Maths::Vector3 gravity;
		float friction;
		float bounce;


#ifdef EULER_INTEGRATION
		Maths::Vector3 velocity;
#endif // EULER_INTEGRATION
		float mass;
	public:
		Particle();

		void Update(float deltaTime);
		virtual void Render(const class Render::Renderer* render, class Render::Camera* camera,
			class Render::LightManager* lightManager);

		void ApplyForces();
		void AddForce(const Maths::Vector3& force);
		void SolveContraints(const std::vector<Geometry3D::OBB>& obb);


		void SetPosition(const Maths::Vector3 pos);
		Maths::Vector3 GetPosition();

		void SetBounce(float b);
		float GetBounce();

		void AddImpulse(const Maths::Vector3& impulse);
		float invMass();
		void SetMass(float m);
		Maths::Vector3 GetVelocity();
		void SetFriction(float f);


	};
}
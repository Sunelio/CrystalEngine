#include "Physics/Particle.h"
#include "Physics/Geometry3D.h"
#include "Render/Renderer.h"
#include "Resources/Material.h"

#include "glad/glad.h"

using namespace Maths;
using namespace Geometry3D;
using namespace Render;


namespace Physic
{
	Particle::Particle()
	{
		type = RIGIDBODY_TYPE_PARTICLE;
		friction = 0.95f;
		bounce = 0.7f;
		gravity = Vector3(0.0f, -9.82f, 0.0f);
#ifdef EULER_INTEGRATION
		mass = 1.0f;
#endif // EULER_INTEGRATION

	}

	void Particle::Update(float deltaTime)
	{
#ifdef EULER_INTEGRATION
		oldPosition = position;
		Vector3 acceleration = forces * invMass();
#ifdef ACCURATE_EULER_INTEGRATION
		Vector3 oldVelocity = velocity;
		velocity = velocity * friction + acceleration * deltaTime;
		position = position + (oldVelocity + velocity) * 0.5f * deltaTime;
#else
		velocity = velocity * friction + acceleration * deltaTime;
		position = position + velocity * deltaTime;

#endif // ACCURATE_EULER_INTEGRATION
#else
		Vector3 velocity = position - oldPosition;
		oldPosition = position;
		float deltaSquare = deltaTime * deltaTime;
		position = position + (velocity * friction + forces * deltaSquare);

#endif // EULER_INTEGRATION

	}

	void Particle::Render(const class Render::Renderer* render, class Render::Camera* camera,
		class Render::LightManager* lightManager)
	{
		//NOTE(o.luanda): Render particle



	}

	void Particle::ApplyForces()
	{
#ifdef EULER_INTEGRATION
		forces = gravity * mass;
#else
		forces = gravity * mass;
#endif // EULER_INTEGRATION


	}

	void Particle::AddForce(const Maths::Vector3& force)
	{
		forces += force;
	}

	void Particle::SolveContraints(const std::vector<Geometry3D::OBB>& constraints)
	{
		const size_t size = constraints.size();
		for (size_t i = 0; i < size; ++i) {
			Line traveled(oldPosition, position);
			if (Linetest(constraints[i], traveled)) {
				//if (PointInOBB(position, constraints[i])) {
#ifndef EULER_INTEGRATION
				Vector3 velocity = position - oldPosition;
#endif
				Vector3 direction = velocity.GetNormalized();
				Ray ray(oldPosition, direction);
				RaycastResult result;

				if (Raycast(constraints[i], ray, &result)) {
					// Place object just a little above collision result
					position = result.point + result.normal * 0.003f;

					Vector3 vn = result.normal * result.normal.Dot(velocity);
					Vector3 vt = velocity - vn;

#ifdef EULER_INTEGRATION
					oldPosition = position;
					velocity = vt - vn * bounce;
#else
					oldPosition = position - (vt - vn * bounce);
#endif
					break;
				}
			}
		}
	}


	void Particle::SetPosition(const Maths::Vector3 pos)
	{
		position = pos;
		oldPosition = pos;
	}

	Maths::Vector3 Particle::GetPosition()
	{
		return position;
	}

	void Particle::SetBounce(float b)
	{
		bounce = b;
	}

	float Particle::GetBounce()
	{
		return bounce;
	}

	void Particle::AddImpulse(const Maths::Vector3& impulse)
	{
#ifdef EULER_INTEGRATION
		velocity = velocity + impulse;
#else
		Vector3 velocity = position - oldPosition;
		velocity = velocity + impulse;
		oldPosition = position - velocity;
#endif
	}

	float Particle::invMass()
	{
		if (mass == 0.0f) { return 0.0f; }
		return 1.0f / mass;
	}

	void Particle::SetMass(float m)
	{
		mass = m;
	}
	Maths::Vector3 Particle::GetVelocity()
	{
#ifdef EULER_INTEGRATION
		return velocity;
#else
		return position - oldPosition;
#endif
	}

	void Particle::SetFriction(float f)
	{
		friction = f;
	}
}
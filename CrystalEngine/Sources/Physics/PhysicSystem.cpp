#pragma warning(disable: 4267)
#pragma warning(disable: 4002)

#include "Physics/PhysicSystem.h"
#include "Physics/Geometry3D.h"
#include "Physics/RigidbodyVolume.h"
#include "glad/glad.h"
#include "Maths/Vector3.h"
#include "Render/LightManager.h"
#include "Render/CameraManager.h"
#include "Resources/ResourceManager.h"
#include "Render/Renderer.h"

using namespace Resources;
using namespace Maths;
using namespace Geometry3D;
using namespace Render;

namespace Physic
{
	PhysicSystem::PhysicSystem(int pnumColliders)

		:numColliders(pnumColliders)
	{
		debugRender = false;
		doLinearProjection = true;

		linearProjectionPercent = 0.45f;
		penetrationSlack = 0.01f;
		impulseIteration = 5;

		colliders1.reserve(numColliders);
		colliders2.reserve(numColliders);
		results.reserve(numColliders);

	}

	void PhysicSystem::Update(float deltaTime)
	{
		//NOTE(o.luanda) : Fixed delta time to update physics
		float fixedMillis = 3.0f / 1000.0f;
		float fixedEllapsed = 0.0f;

		fixedEllapsed += deltaTime;

		while (fixedEllapsed > fixedMillis)
		{
			FixedUpdate(fixedMillis);
			fixedEllapsed -= fixedMillis;
		}
#if 0
		FixedUpdate(deltaTime);

#endif
	}

	void PhysicSystem::FixedUpdate(float deltaTime)
	{
		colliders1.clear();
		colliders2.clear();
		//triggers.clear();
		results.clear();

		{	// Find objects whom are colliding
			// First, build a list of colliding objects
			CollisionManifold result;
			for (int i = 0, size = bodies.size(); i < size; ++i)
			{
				for (int j = i; j < size; ++j)
				{
					if (i == j)
					{
						continue;
					}

					if (bodies[i]->isTrigger || bodies[j]->isTrigger)
					{
						continue;
					}

					ResetCollisionManifold(&result);
					if (bodies[i]->HasVolume() && bodies[j]->HasVolume())
					{
						RigidbodyVolume* m1 = (RigidbodyVolume*)bodies[i];
						RigidbodyVolume* m2 = (RigidbodyVolume*)bodies[j];
						result = FindCollisionFeatures(*m1, *m2);
					}
					if (result.colliding)
					{

#if 0
						if (bodies[j]->isTrigger)
						{

							triggers.push_back(bodies[j]);
							if (bodies[i]->isTrigger)
							{
								triggers.push_back(bodies[i]);
								results.push_back(result);

							}
						}
						if (!bodies[i]->isTrigger && !bodies[j]->isTrigger)
						{
							colliders1.push_back(bodies[i]);
							colliders2.push_back(bodies[j]);
							results.push_back(result);
						}

#endif


#if 1
						{
							colliders1.push_back(bodies[i]);
							colliders2.push_back(bodies[j]);
							results.push_back(result);
						}
#endif
			}
		}
	}
	}





		for (int i = 0, size = bodies.size(); i < size; ++i)
		{
			bodies[i]->ApplyForces();
		}


		// Apply impulses to resolve collisions
		for (int k = 0; k < impulseIteration; ++k) // Apply impulses 
		{ 
			for (int i = 0, size = results.size(); i < size; ++i) 
			{
				for (int j = 0, jSize = results[i].contacts.size(); j < jSize; ++j) 
				{
					if (colliders1[i]->HasVolume() && colliders2[i]->HasVolume()) 
					{
						RigidbodyVolume* m1 = (RigidbodyVolume*)colliders1[i];
						RigidbodyVolume* m2 = (RigidbodyVolume*)colliders2[i];
						ApplyImpulse(*m1, *m2, results[i], j);
						//if(m1->isTrigger && m2->isTrigger)
					}
				}
			}
		}


		// Integrate velocity and impulse of objects
		for (int i = 0, size = bodies.size(); i < size; ++i)
		{
			bodies[i]->Update(deltaTime);
		}

		// Correct position to avoid sinking!
		if (doLinearProjection) 
		{
			for (int i = 0, size = results.size(); i < size; ++i) {
				if (!colliders1[i]->HasVolume() && !colliders2[i]->HasVolume())
				{
					continue;
				}

				RigidbodyVolume* m1 = (RigidbodyVolume*)colliders1[i];
				RigidbodyVolume* m2 = (RigidbodyVolume*)colliders2[i];
				float totalMass = m1->InvMass() + m2->InvMass();

				if (totalMass == 0.0f) {
					continue;
				}

				float depth = fmaxf(results[i].depth - penetrationSlack, 0.0f);
				float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
				Vector3 correction = results[i].normal * scalar * linearProjectionPercent;

				m1->position = m1->position - correction * m1->InvMass();
				m2->position = m2->position + correction * m2->InvMass();

				m1->SynchCollisionVolumes();
				m2->SynchCollisionVolumes();
			}
		}


		for (int i = 0, size = bodies.size(); i < size; ++i) 
		{
			bodies[i]->SolveConstraints(constraints);
		}


	}

	void PhysicSystem::AddRigidBody(RigidBody* body)
	{
		bodies.emplace_back(body);
	}

	void PhysicSystem::AddConstraint(const Geometry3D::OBB& constraint)
	{
		constraints.push_back(constraint);
	}

	void PhysicSystem::ClearRigidBodys()
	{
		bodies.clear();
		triggers.clear();
	}

	void PhysicSystem::ClearContraints()
	{
		constraints.clear();
	}

	void PhysicSystem::Render(const Render::Renderer* render, Render::Camera* camera, Render::LightManager* lightManager)
	{

		//NOTE(o.luanda)
		//Normally, we don't need to render physics
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (int i = 0, size = bodies.size(); i < size; ++i)
		{
			if (debugRender && bodies[i]->type == RIGIDBODY_TYPE_BOX)
			{
				RigidbodyVolume* mb = (RigidbodyVolume*)bodies[i];
				mb->SynchCollisionVolumes();
				mb->Render(render, camera, lightManager);
			}
			else
			{
				bodies[i]->Render(render, camera, lightManager);
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}
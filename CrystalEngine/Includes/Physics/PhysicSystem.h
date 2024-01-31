#pragma once
#include "Physics/RigidBody.h"
#include "RenderHelp.h"
#include <vector>

namespace Render
{
	class Renderer;
	class CameraManager;
	class Camera;
	class LightManager;
}

namespace Physic
{
	class PhysicSystem
	{
	public:
		bool debugRender;
		bool renderRandomColors;
		int numColliders;      //NOTE(o.luanda)Maybe put it in private ????????
		bool doLinearProjection;
		float penetrationSlack;
		float linearProjectionPercent;
		float impulseIteration;

	private:


	protected:
		std::vector<RigidBody*> bodies;
		std::vector<Geometry3D::OBB> constraints;

		std::vector<RigidBody*> colliders1;
		std::vector<RigidBody*> colliders2;
		std::vector<RigidBody*> triggers;
		std::vector<Geometry3D::CollisionManifold> results;


	public:
		PhysicSystem(int pnumColliders = 100);

		void Update(float deltaTime);
		void Render(const Render::Renderer* render, Render::Camera* camera, Render::LightManager* lightManager);

		void AddRigidBody(RigidBody* body);
		void AddConstraint(const Geometry3D::OBB& constraint);

		void ClearRigidBodys();
		void ClearContraints();

	private:
		void FixedUpdate(float deltaTime);

	};

}
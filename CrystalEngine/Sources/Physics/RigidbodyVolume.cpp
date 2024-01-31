#include "Physics/RigidbodyVolume.h"
#include "Maths/Arithmetic.h"
#include "Physics/Geometry3D.h"
#include "Maths/Matrix.h"
#include "Scenes/GameObject.h"
#include "Maths/Matrix.h"

#include "Physics/RenderHelp.h"



using namespace Core;
using namespace Render;
using namespace Resources;
using namespace Scenes;
using namespace Maths;
using namespace Geometry3D;


#define InternalFunction static
#define InternalGlobalVariable static






#define CMP(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))



namespace Physic
{
	InternalFunction Mat3 YRotation3x3(float angle) {
		angle = radToDeg(angle);
		return Mat3(
			cosf(angle), 0.0f, -sinf(angle),
			0.0f, 1.0f, 0.0f,
			sinf(angle), 0.0f, cosf(angle)
		);
	}

	InternalFunction Mat3 XRotation3x3(float angle)
	{
		angle = radToDeg(angle);
		return Mat3(
			1.0f, 0.0f, 0.0f,
			0.0f, cosf(angle), sinf(angle),
			0.0f, -sinf(angle), cosf(angle)
		);
	}

	InternalFunction Mat3 ZRotation3x3(float angle)
	{
		angle = radToDeg(angle);
		return Mat3(
			cosf(angle), sinf(angle), 0.0f,
			-sinf(angle), cosf(angle), 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	InternalFunction Mat3 Rotation3x3(float pitch, float yaw, float roll)
	{
		return ZRotation3x3(roll) * XRotation3x3(pitch) * YRotation3x3(yaw);
	}

	InternalFunction Vector3 MultiplyVector(const Vector3& vec, const Mat4& mat)
	{
		Vector3 result;
		result.x = vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2] + 0.0f * mat.m[0][3];
		result.y = vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2] + 0.0f * mat.m[1][3];
		result.z = vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2] + 0.0f * mat.m[2][3];
		return result;
	}


	void RigidbodyVolume::ApplyForces()
	{
		if (mass <= 0.0f)
			return;

		if (takeGravity)
		{
			forces = GRAVITY_CONSTANT * mass;
		}
	}

	void RigidbodyVolume::AddForce(const Maths::Vector3& force)
	{
		forces += force;
	}



#ifndef LINEAR_ONLY
	void RigidbodyVolume::AddRotationalInpulse(const Maths::Vector3& point, const Maths::Vector3& impulse)
	{
		Vector3 centerOfMass = position;
		Vector3 torque = (point - centerOfMass).Cross(impulse);

		Vector3 angAccel = MultiplyVector(torque, InvTensor());
		angVelocity = angVelocity + angAccel;
	}
#endif // !LINEAR_ONLY

	void RigidbodyVolume::AddLinearImpulse(const Maths::Vector3& impulse)
	{
		velocity = velocity + impulse;
	}

	float RigidbodyVolume::InvMass()
	{
		if (mass == 0.0f)
		{
			return 0.0f;
		}
		return (1.0f / mass);
	}

	void RigidbodyVolume::SynchCollisionVolumes()
	{
		if (type == RIGIDBODY_TYPE_SPHERE)
		{
			sphere.position = owner->transform.GetPosition();
		}
		else if (type == RIGIDBODY_TYPE_BOX)
		{
			box.position = owner->transform.GetPosition();
			if(this->owner->GetType() == GameObjectType::Camera)
				box.size = owner->transform.GetScale() + Vector3(0.7f);

#ifndef LINEAR_ONLY

#endif // !LINEAR_ONLY
		}

	}

	void RigidbodyVolume::Render(const Renderer* render, class Render::Camera* camera, class Render::LightManager* lightManager)
	{
		SynchCollisionVolumes();

		if (type == RIGIDBODY_TYPE_SPHERE)
		{
			//TODO(o.luanda)
		}
		else if (type == RIGIDBODY_TYPE_BOX)
		{
			//TODO(o.luanda)
		}
	}



#ifndef LINEAR_ONLY
	Maths::Mat4 RigidbodyVolume::InvTensor()
	{
		if (mass == 0)
		{
			return Mat4(
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
			);
		}

		float ix = 0.0f;
		float iy = 0.0f;
		float iz = 0.0f;
		float iw = 0.0f;

		if (mass != 0 && type == RIGIDBODY_TYPE_SPHERE)
		{
			float r2 = sphere.radius * sphere.radius;
			float frcation = (2.0f / 5.0f);

			ix = r2 * mass * frcation;
			iy = r2 * mass * frcation;
			iz = r2 * mass * frcation;
			iw = 1.0f;

		}
		else if (mass != 0 && type == RIGIDBODY_TYPE_SPHERE)
		{
			Vector3 size = box.size * 2.0f;
			float fraction = (2.0f / 5.0f);

			float x2 = size.x * size.x;
			float y2 = size.y * size.y;
			float z2 = size.z * size.z;

			ix = (y2 + z2) * mass * fraction;
			iy = (x2 + z2) * mass * fraction;
			iz = (x2 + y2) * mass * fraction;
			iw = 1.0f;
		}

		Mat4 mat(
			ix, 0, 0, 0,
			0, iy, 0, 0,
			0, 0, iz, 0,
			0, 0, 0, iw
		);

		return mat.Inv4();

	}
#endif // LINEAR_ONLY

	void RigidbodyVolume::Update(float deltaTime)
	{
		const float damping = 0.98f;

		Vector3 accelleration = forces * InvMass();
		velocity = velocity + accelleration * deltaTime;
		velocity = velocity * damping;


		if (fabsf(velocity.x) < 0.001f)
		{
			velocity.x = 0.0f;
		}
		if (fabsf(velocity.y) < 0.001f)
		{
			velocity.y = 0.0f;
		}
		if (fabsf(velocity.z) < 0.001f)
		{
			velocity.z = 0.0f;
		}

#ifndef LINEAR_ONLY
		if (type == RIGIDBODY_TYPE_BOX)
		{
			Vector3 result;
			//NOTE(o.luanda):Maybe we need to create a specific function to
			// do that
			//Multiply Vector3 and Mat4
			result.x = torques.Dot(Vector3{ InvTensor().m[0][0], InvTensor().m[1][0], InvTensor().m[2][0] });
			result.y = torques.Dot(Vector3{ InvTensor().m[0][1], InvTensor().m[1][1], InvTensor().m[2][1] });
			result.z = torques.Dot(Vector3{ InvTensor().m[0][2], InvTensor().m[1][2], InvTensor().m[2][2] });
			Vector3 angAccel = result;
			angVelocity = angVelocity + angAccel * deltaTime;
			angVelocity = angVelocity * damping;

			if (fabsf(angVelocity.x) < 0.001f)
			{
				angVelocity.x = 0.0f;
			}
			if (fabsf(angVelocity.y) < 0.001f)
			{
				angVelocity.y = 0.0f;
			}
			if (fabsf(angVelocity.z) < 0.001f)
			{
				angVelocity.z = 0.0f;
			}
		}
#endif // !LINEAR_ONLY



		position = position + velocity * deltaTime;

#ifndef LINEAR_ONLY
		if (type == RIGIDBODY_TYPE_BOX)
		{
			orientation = orientation + angVelocity * deltaTime;
		}

#endif // !LINEAR_ONLY

		SynchCollisionVolumes();


	}


	CollisionManifold FindCollisionFeatures(RigidbodyVolume& ra, RigidbodyVolume& rb)
	{
		CollisionManifold result;
		ResetCollisionManifold(&result);

		if (ra.type == RIGIDBODY_TYPE_SPHERE)
		{
			if (rb.type == RIGIDBODY_TYPE_SPHERE)
			{
				result = FindCollisionFeatures(ra.sphere, rb.sphere);
			}
			else if (rb.type == RIGIDBODY_TYPE_BOX)
			{
				result = FindCollisionFeatures(rb.box, ra.sphere);
				result.normal = result.normal * -1.0f;
			}
		}
		else if (ra.type == RIGIDBODY_TYPE_BOX)
		{
			if (rb.type == RIGIDBODY_TYPE_BOX)
			{
				result = FindCollisionFeatures(ra.box, rb.box);
			}
			else if (rb.type == RIGIDBODY_TYPE_SPHERE)
			{
				result = FindCollisionFeatures(ra.box, rb.sphere);
			}
		}


		return result;
	}
	//TODO(o.luanda)
	void ApplyImpulse(RigidbodyVolume& A, RigidbodyVolume& B, const CollisionManifold& M, int c)
	{
		// Linear impulse
		float invMass1 = A.InvMass();
		float invMass2 = B.InvMass();
		float invMassSum = invMass1 + invMass2;

		if (invMassSum == 0.0f) {
			return; // Both objects have infinate mass!
		}

#ifndef LINEAR_ONLY
		Vector3 r1 = M.contacts[c] - A.position;
		Vector3 r2 = M.contacts[c] - B.position;
		Mat4 i1 = A.InvTensor();
		Mat4 i2 = B.InvTensor();
#endif

		// Relative velocity
#ifndef LINEAR_ONLY
		Vector3 relativeVel = (B.velocity + B.angVelocity.Cross(r2)) - (A.velocity + A.angVelocity.Cross(r1));
#else
		Vector3 relativeVel = B.velocity - A.velocity;
#endif
		// Relative collision normal
		Vector3 relativeNorm = M.normal;
		relativeNorm.Normalize();

		// Moving away from each other? Do nothing!
		if (relativeVel.Dot(relativeNorm) > 0.0f) 
		{
			return;
		}

		float e = fminf(A.cor, B.cor);

		float numerator = (-(1.0f + e) * relativeVel.Dot(relativeNorm));
		float d1 = invMassSum;
#ifndef LINEAR_ONLY
		Vector3 d2 = MultiplyVector(r1.Cross(relativeNorm), i1).Cross(r1);
		Vector3 d3 = MultiplyVector(r2.Cross(relativeNorm), i2).Cross(r2);
		float denominator = d1 + relativeNorm.Dot(d2 + d3);
#else
		float denominator = d1;
#endif

		float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;
		if (M.contacts.size() > 0.0f && j != 0.0f) {
			j /= (float)M.contacts.size();
		}

		Vector3 impulse = relativeNorm * j;
		A.velocity = A.velocity - impulse * invMass1;
		B.velocity = B.velocity + impulse * invMass2;

#ifndef LINEAR_ONLY
		A.angVelocity = A.angVelocity - MultiplyVector(r1.Cross(impulse), i1);
		B.angVelocity = B.angVelocity + MultiplyVector(r2.Cross(impulse), i2);
#endif

		// Friction
		Vector3 t = relativeVel - (relativeNorm * relativeVel.Dot(relativeNorm));
		if (CMP(t.GetLengthSq(), 0.0f)) {
			return;
		}
		t.Normalize();

		numerator = -relativeVel.Dot(t);
		d1 = invMassSum;
#ifndef LINEAR_ONLY
		d2 = MultiplyVector(r1.Cross(t), i1).Cross(r1);
		d3 = MultiplyVector(r2.Cross(t), i2).Cross(r2);
		denominator = d1 + t.Dot(d2 + d3);
#else
		denominator = d1;
#endif

		float jt = (denominator == 0.0f) ? 0.0f : numerator / denominator;
		if (M.contacts.size() > 0.0f && jt != 0.0f) {
			jt /= (float)M.contacts.size();
		}

		if (CMP(jt, 0.0f)) {
			return;
		}

		Vector3 tangentImpuse;
#ifdef DYNAMIC_FRICTION
		float sf = sqrtf(A.staticFriction * B.staticFriction);
		float df = sqrtf(A.dynamicFriction * B.dynamicFriction);
		if (fabsf(jt) < j * sf) {
			tangentImpuse = t * jt;
		}
		else {
			tangentImpuse = t * -j * df;
		}
#else
		float friction = sqrtf(A.friction * B.friction);
		if (jt > j * friction) {
			jt = j * friction;
		}
		else if (jt < -j * friction) {
			jt = -j * friction;
		}
		tangentImpuse = t * jt;
#endif

		A.velocity = A.velocity - tangentImpuse * invMass1;
		B.velocity = B.velocity + tangentImpuse * invMass2;

#ifndef LINEAR_ONLY
		A.angVelocity = A.angVelocity - MultiplyVector(r1.Cross(tangentImpuse), i1);
		B.angVelocity = B.angVelocity + MultiplyVector(r2.Cross(tangentImpuse), i2);
#endif
	}

}


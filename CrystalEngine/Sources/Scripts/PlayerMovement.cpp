#include "Scripts/PlayerMovement.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"
#include "Scenes/Scene.h"

#include "Render/Camera.h"
#include "Render/CameraManager.h"

#include "Render/Light.h"

#include "Core/Engine.h"
#include "Core/Events.h"
#include "Core/EventDispatcher.h"
#include "Core/TimeManager.h"
#include "Core/Window.h"

#include "Maths/Maths.h"
#include "Physics/RigidbodyVolume.h"
#include "Physics/PhysicSystem.h"
#include "Refureku/Generated/PlayerMovement.rfks.h"

#include <string>

using namespace Scripts;
using namespace Physic;
using namespace Scenes;
using namespace Core;
using namespace Maths;

PlayerMovement::PlayerMovement() { archId = staticGetArchetype().getId(); }

void PlayerMovement::Start()
{
	sensibility = 1;

	object->rigidBody = new RigidbodyVolume(object, RIGIDBODY_TYPE_BOX);
	object->rigidBody->position = object->transform.GetPosition();
	object->rigidBody->box.size = Vector3(0.7f);
	object->rigidBody->mass = 2.5f;
	object->rigidBody->takeGravity = false;

	object->rigidBody->SynchCollisionVolumes();
	engine->GetSceneManager()->GetActiveScene()->GetPhysicsSystem()->AddRigidBody(object->rigidBody);
}

void PlayerMovement::Update()
{

	if (state == PlayerState::Alive)
	{
		static Window* window = Engine::Get()->GetWindow();

		//light->transform = object->transform;

		if (inputs.shiftpress) speed = 5;
		else speed = 3;

		Vector2 mouseDelta = inputs.mouseDelta;
		Vector3 rotateMove = transform->GetRotation().RotateVec((inputs.movement * speed * time->DeltaTime()));

		float rotUpClamp = transform->Forward().Dot(Vector3(0, 1, 0));
		float rotDownClamp = transform->Forward().Dot(Vector3(0, -1, 0));

		if (rotUpClamp > 0.95f)	  mouseDelta.y = clampUnder(mouseDelta.y, 0);
		if (rotDownClamp > 0.95f) mouseDelta.y = clampAbove(mouseDelta.y, 0);

		window->LockHideCursor(true);

		transform->Move(Vector3(rotateMove.x, 0, rotateMove.z));
		transform->Rotate(Quaternion::FromAngleAxis({ -mouseDelta.y * sensibility * 1e-3f, transform->Right() }));
		transform->Rotate(Quaternion::FromRoll(-mouseDelta.x * sensibility * 1e-3f));

		//Update rigidbody position if it exist
		if (object->rigidBody)
		{
			object->rigidBody->position = transform->GetPosition();
		}
	}
}

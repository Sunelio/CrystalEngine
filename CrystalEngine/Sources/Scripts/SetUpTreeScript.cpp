#include "Scripts/SetUpTreeScript.h"

#include "Core/Engine.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"
#include "Scenes/Scene.h"

#include "Physics/RigidbodyVolume.h"
#include "Physics/PhysicSystem.h"
#include "Physics/Geometry3D.h"

#include "Refureku/Generated/SetUpTreeScript.rfks.h"

using namespace Scripts;
using namespace Physic;
using namespace Scenes;
using namespace Core;
using namespace Maths;

SetUpTreeScript::SetUpTreeScript(){ archId = staticGetArchetype().getId(); }

void SetUpTreeScript::Start()
{
    object->rigidBody = new RigidbodyVolume(object, RIGIDBODY_TYPE_BOX);
    object->rigidBody->position = object->transform.GetPosition();
    object->rigidBody->box.size = Vector3(0.7f);
    object->rigidBody->mass = 0.0f;
    object->rigidBody->takeGravity = false;
    
    object->rigidBody->SynchCollisionVolumes();
    engine->GetSceneManager()->GetActiveScene()->GetPhysicsSystem()->AddRigidBody(object->rigidBody);
}
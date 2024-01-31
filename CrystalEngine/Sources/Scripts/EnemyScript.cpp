#include "Scripts/EnemyScript.h"

#include "Resources/Mesh.h"
#include "Resources/ResourceManager.h"

#include "Core/SoundManager.h"
#include "Core/Engine.h"

#include "Scenes/SceneGraph.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/GameObject.h"

#include "Animation/Animator.h"
#include "Animation/Animation.h"

#include  "Maths/Maths.h"

#include "Core/TimeManager.h"
#include "Physics/RigidbodyVolume.h"
#include "Physics/PhysicSystem.h"
#include "Physics/Geometry3D.h"

#include "Refureku/Generated/EnemyScript.rfks.h"
#include "Scripts/PlayerMovement.h"

using namespace Scripts;
using namespace Scenes;
using namespace Anim;
using namespace Maths;
using namespace Physic;
using namespace Geometry3D;
using namespace Resources;

EnemyScript::EnemyScript(){ archId = staticGetArchetype().getId(); }

void EnemyScript::Start()
{
	soundObject = (SoundObject*)scene->GetSceneGraph()->Find("SoundManager");

	idleAnimation = "Armature.001|Idle";
	runAnimation =  "Armature.001|Run";

	mainSongName  =	"Resources/Sounds/Dark-Forest.mp3";
	chaseSongName = "Resources/Sounds/Chase-1.mp3";
	deadSongName  =	"Resources/Sounds/Jumpscare.mp3";

	mainSong =	engine->GetResourceManager()->Get<Sound>(mainSongName);
	chaseSong = engine->GetResourceManager()->Get<Sound>(chaseSongName);
	deadSong =	engine->GetResourceManager()->Get<Sound>(deadSongName);

	chaseSong->SetLooping(true);
	mainSong->SetLooping(true);

	soundObject->AddSound(mainSongName);
	soundObject->AddSound(chaseSongName);
	soundObject->AddSound(deadSongName);

	if (!((ModelObject*)object)) return;

	animator = ((ModelObject*)object)->mesh->GetAnimator();
	player   = scene->GetSceneGraph()->Find("Camera");
}

void EnemyScript::Update()
{
	if (playerDetected) ChaseMode();
	else                IdleMode();

	const float playerDist = Vector3(transform->GetPosition(), player->transform.GetPosition()).GetLength();

	if (playerDist < killRadius)
	{
		if (PlayerMovement* script = (PlayerMovement*)player->GetScript(0))
		{
			if (script->state == PlayerState::Alive)
			{
				animator->SetCurrentAnimation(idleAnimation);

				if (soundObject->GetCurrentSound())
					soundObject->GetCurrentSound()->Stop();

				soundObject->SetCurrentSound(deadSong);
				soundObject->PlayCurrentSound();
				
				script->state = PlayerState::Dead;
			}
		}
	}
	
	playerDetected = playerDist < detectRadius;

	if (playerDist > teleportRadius)
	{
		Vector3 horizontalForward = player->transform.Forward(); horizontalForward.y = 0; horizontalForward.Normalize();
		transform->SetPosition(player->transform.GetPosition() + horizontalForward * (detectRadius - 5));
	}
}

void EnemyScript::IdleMode()
{
	if (chasing)
	{
		animator->SetCurrentAnimation(idleAnimation);

		if (soundObject->GetCurrentSound())
			soundObject->GetCurrentSound()->Stop();

		soundObject->SetCurrentSound(mainSong);
		soundObject->PlayCurrentSound();

		chasing = false;
	}
}

void EnemyScript::ChaseMode()
{
	if (!chasing)
	{
		animator->SetCurrentAnimation(runAnimation);
		animator->GetCurrentAnimation()->speed = 30;

		if (soundObject->GetCurrentSound())
			soundObject->GetCurrentSound()->Stop();

		soundObject->SetCurrentSound(chaseSong);
		soundObject->PlayCurrentSound();
		
		chasing = true;
	}

	Vector3 playerDir = { transform->GetPosition(), player->transform.GetPosition() }; playerDir.y = 0; playerDir.Normalize();
	Vector3 upAxis    = { 0, 1, 0 };
	Vector3 sideAxis  = upAxis.Cross(playerDir);
	const Mat3 matrix = { sideAxis.x, sideAxis.y, sideAxis.z,
		                  upAxis.x, upAxis.y, upAxis.z,	
	                      playerDir.x, playerDir.y, playerDir.z };
	Vector3 euler = matrix.ToQuaternion().GetNormalized().ToEuler();
	euler.y *= -1;
	
	transform->Move(playerDir * speed * time->DeltaTime());
	// transform->SetRotation(matrix.ToQuaternion().GetNormalized());
	transform->SetEulerRot(euler);
}



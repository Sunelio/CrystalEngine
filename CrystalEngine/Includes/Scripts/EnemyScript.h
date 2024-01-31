#pragma once
#include "Scripts/ObjectScript.h"
#include <Refureku/Generated/EnemyScript.rfkh.h>

namespace Resources { class Mesh;       }
namespace Anim      { class Animator;   }
namespace Scenes    { class GameObject; class SoundObject; }
namespace Resources { class Sound;      } 

namespace Scripts NAMESPACE()
{
    struct STRUCT() EnemyScript : ObjectScript
    {
        FIELD() bool playerDetected = false;
        FIELD() bool chasing = false;

        FIELD() float speed          = 2;
        FIELD() float killRadius     = 4;
        FIELD() float detectRadius   = 40;
        FIELD() float teleportRadius = 55;

        FIELD() Anim::Animator*      animator    = nullptr;
        FIELD() Scenes::GameObject*  player      = nullptr;
        FIELD() Resources::Sound*    mainSong    = nullptr;
        FIELD() Resources::Sound*    chaseSong   = nullptr;
        FIELD() Resources::Sound*    deadSong    = nullptr;
        FIELD() Scenes::SoundObject* soundObject = nullptr;

        FIELD()  std::string mainSongName;
        FIELD()  std::string chaseSongName;
        FIELD()  std::string deadSongName;
        FIELD()  std::string runAnimation;
        FIELD()  std::string idleAnimation;

        METHOD() EnemyScript();

        METHOD() void IdleMode();
        METHOD() void ChaseMode();

        METHOD() void Start() override;
        METHOD() void Update() override;

        Scripts_EnemyScript_GENERATED
    };
}

File_EnemyScript_GENERATED
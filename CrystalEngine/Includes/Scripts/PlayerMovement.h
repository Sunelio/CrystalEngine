#pragma once
#include "Scripts/ObjectScript.h"
#include <Refureku/Generated/PlayerMovement.rfkh.h>

namespace Render { class Camera; }
namespace Scenes { class GameObject; }

using namespace Scenes;

namespace Scripts NAMESPACE()
{
    enum class PlayerState
    {
       Alive,
       Dead
    };
    
    struct STRUCT() PlayerMovement : ObjectScript
    {
        FIELD() float speed;
        FIELD() float sensibility;
        FIELD() GameObject* light = nullptr;
        FIELD() PlayerState state =  PlayerState::Alive;
        
        METHOD() PlayerMovement();
        METHOD() void Start() override;
        METHOD() void Update() override;

        Scripts_PlayerMovement_GENERATED
    };
}

File_PlayerMovement_GENERATED
#pragma once
#include "Scripts/ObjectScript.h"
#include <Refureku/Generated/MainMenu.rfkh.h>

namespace Scenes { class GameObject; class SoundObject; }
namespace Resources { class Sound; }

namespace Scripts NAMESPACE()
{
	struct STRUCT() MainMenu : ObjectScript
	{
		FIELD() Resources::Sound* mainSound;

		METHOD() MainMenu();
		METHOD() void Start() override;
		METHOD() void Update() override;

		Scripts_MainMenu_GENERATED
	};
}

File_MainMenu_GENERATED
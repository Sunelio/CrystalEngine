#pragma once
#include "Scripts/ObjectScript.h"
#include <Refureku/Generated/SetUpTreeScript.rfkh.h>

namespace Render { class Camera; }
namespace Scenes { class GameObject; }

using namespace Scenes;

namespace Scripts NAMESPACE()
{
    struct STRUCT() SetUpTreeScript : ObjectScript
    {


        METHOD() SetUpTreeScript();
        METHOD() void Start() override;

        Scripts_SetUpTreeScript_GENERATED
    };
}

File_SetUpTreeScript_GENERATED

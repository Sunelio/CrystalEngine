#include "Scripts/ShaderScript.h"
#include "Core/Engine.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"
#include "Refureku/Generated/ShaderScript.rfks.h"

using namespace Scripts;
using namespace Core;

ShaderScript::ShaderScript()
{
    const Engine* _engine = Engine::Get();
    engine   = _engine;
    renderer = engine->GetRenderer();


    
}

ScreenShaderScript::ScreenShaderScript()
{
    postProcessor = engine->GetPostProcessor();
}

#include "Core/Engine.h"
#include "Core/ThreadManager.h"
#include "Core/Window.h"
#include "Render/Renderer.h"
#include "Scenes/SceneManager.h"

using namespace Core;

int main()
{
    Engine* engine = Engine::Create();
    engine->Init({ "CrystalGame" });
    engine->GetSceneManager()->StartActiveScene();
    engine->GetRenderer()->RenderObjectsToStencil(true);

    while (!engine->GetWindow()->ShouldClose())
    {
        engine->ExecuteFrame();
        engine->RenderToScreen();
        engine->EndFrame();
    }

    engine->Release();
    Engine::Destroy();
}


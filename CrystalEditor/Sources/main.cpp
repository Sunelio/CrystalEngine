#include "Core/Engine.h"
#include "Core/Window.h"
#include "UI/Editor.h"
using namespace Core;
using namespace UI;

int main()
{
    // Initialization.
    Engine* engine = Engine::Create();
    engine->Init({ "CrystalEngine" });
    Editor* editor = new Editor(engine);

    // Update & render loop.
    while(!engine->GetWindow()->ShouldClose())
    {
        editor->LockInputsIfUsed();
        engine->ExecuteFrame();
        editor->ExecuteFrame();
        editor->EndFrame();
        engine->EndFrame();
    }

    // De-initialization.
    delete editor;
    engine->Release();
    Engine::Destroy();
}

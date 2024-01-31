#include "Core/Engine.h"
#include "Core/EventDispatcher.h"
#include "Core/ThreadManager.h"
#include "Debug/Logger.h"
#include "Core/Window.h"
#include "Core/TimeManager.h"
#include "Render/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "Render/PostProcessor.h"
#include "Resources/Sound.h"
#include "Core/SoundManager.h"
#include "Scenes/Scene.h"
using namespace Core;
using namespace Resources;
using namespace Scenes;
using namespace Render;

Engine* Engine::Create()
{
    if (!instance)
        instance = new Engine;
    return instance;
}

Engine* Engine::Get()
{
    return instance;
}

void Engine::Destroy()
{
    delete instance;
}

void Engine::Init(const WindowParams& windowParams)
{
    eventDispatcher = new EventDispatcher;
    logger          = new Debug::Logger("Resources/engine.log");
    Window::InitGlfw();
    window          = new Window(windowParams);
    time            = new TimeManager(120, false);
    threadManager   = new ThreadManager();
    soundManager    = new SoundManager();
    resourceManager = new ResourceManager();
    resourceManager->LoadDefaultResources();
    
    renderer        = new Renderer();
    postProcessor   = new PostProcessor();

    sceneManager    = new SceneManager();
}

void Engine::ExecuteFrame() const
{
    while (!time->CanStartNextFrame()) continue;
        
    time           ->NewFrame();
    threadManager  ->Update();
    eventDispatcher->Update(time->DeltaTime());
    window         ->Update(time->DeltaTime());

    if (const Scene* scene = sceneManager->GetActiveScene())
    {
        scene        ->Update(time->DeltaTime(), window->GetInputs());
        postProcessor->BeginRender(scene->GetRenderTexture());
        scene        ->Render(renderer);
        postProcessor->EndRender(scene->GetRenderTexture());
    }
}

void Engine::EndFrame() const
{
    window->SwapBuffers();
}

void Engine::RenderToScreen() const 
{
    renderer->UnbindRenderTexture();
    if (const Scene* scene = sceneManager->GetActiveScene())
        renderer->DrawScreenTexture(scene->GetRenderTexture());
}

void Engine::Quit() const
{
    window->Close();
}

void Engine::Release() const
{
    delete sceneManager;
    delete resourceManager;
    delete renderer;
    delete time;
    delete window;
    Window::TerminateGlfw();
    delete logger;
}

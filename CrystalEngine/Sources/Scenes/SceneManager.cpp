#include "Scenes/SceneManager.h"

#include <filesystem>
#include <fstream>

#include "Core/Engine.h"
#include "Core/EventDispatcher.h"
#include "Core/ThreadManager.h"
#include "Scenes/Scene.h"
#include "Core/Window.h"
#include "Debug/Logger.h"
#include "Utils/WindowsConverter.h"

using namespace Scenes;

const std::string defaultSceneTemplate = R"({"Resources": ["Resources/Textures/UI/Button/ButtonPress.png","Resources/Textures/UI/Button/ButtonHovered.png","Resources/Textures/UI/Button/DefaultButton.png","Resources/Fonts/Minecraft.ttf"],"CameraManager": {"engineCameraSensitivity": 1.500000,"engineCameraSpeed": 2.000000,"cameras": [{"id": 1,"near": 0.010000,"width": 1920,"height": 1017,"far": 100.000000,"fov": 60.000000}],"engineCamera": {"id": 0,"near": 0.010000,"width": 1920,"height": 1017,"far": 100.000000,"fov": 60.000000},"sceneCamera": 1,"engineCameraTransform": {"position": [0.000000,0.000000,0.000000],"rotation": [1.000000,0.000000,0.000000,0.000000],"isCamera": true}},"LightManager": {"lights": []},"PostProcessor": [],"SceneGraph": {"rootObject": 0,"gameObjects": [{"id": 1,"name": "Skybox","cubemap": "forest_cubemap","parent": 0,"type": "Cubemap","scripts": [],"transform": {"position": [0.000000,0.000000,0.000000],"rotation": [1.000000,0.000000,0.000000,0.000000],"scale": [50.000000,50.000000,50.000000]}},{"id": 2,"name": "Camera","camera": 1,"parent": 0,"type": "Camera","scripts": [],"transform": {"position": [0.000000,0.000000,0.000000],"rotation": [1.000000,0.000000,0.000000,0.000000],"isCamera": true}}],"objectTags": []}})";

SceneManager::SceneManager()
{
    engine = Core::Engine::Get();
    /*
    const std::filesystem::path defaultScene = "Resources/Scenes/DefaultScene.json";
    std::ofstream stream(absolute(defaultScene).string());
    stream << defaultSceneTemplate;
    stream.close();
    */
    Create("Resources/Scenes/ExampleScene.json")->Load(false);
    activeScene = scenes["Resources/Scenes/ExampleScene.json"];

    // Setup event callbacks.
    Core::Engine::Get()->GetEventDispatcher()->AddListener<Core::ReloadSceneEvent>([this](Core::Event*)
    {
        if (activeScene) {
            activeScene->Unload();
            activeScene->Load(false);
        }
    });
    Core::Engine::Get()->GetEventDispatcher()->AddListener<Core::SetActiveSceneEvent>([this](Core::Event* event)
    {
        const std::string name = ((Core::SetActiveSceneEvent*)event)->name;
        const bool shouldStart = ((Core::SetActiveSceneEvent*)event)->shouldStart;
        if (!Exists(name)) return;
        
        if (!Loaded(name)) {
            std::function loadTask = [this, name, shouldStart]
            {
                scenes[name]->Load(false);
                engine->GetEventDispatcher()->FrameEndDispatch(new Core::SetActiveSceneEvent(name, shouldStart));
            };
            engine->GetThreadManager()->AddTask(loadTask);
            return;
        }
        activeScene = scenes[name];
        if (shouldStart) activeScene->Start();
    });
}

SceneManager::~SceneManager()
{
    for (auto [name, scene] : scenes)
        delete scene;
    scenes.clear();
}

bool SceneManager::Exists(const std::string& name) const
{
    return scenes.count(name) > 0;
}

bool SceneManager::Loaded(const std::string& name) const
{
    if (!Exists(name))
        return false;
    return scenes.at(name)->IsLoaded();
}

Scene* SceneManager::Create(const std::string& name)
{
    if (Exists(name)) {
        DebugLogWarning("Can't create 2 scenes with the same name: " + name);
        return scenes[name];
    }
    Scene* newScene = new Scene(name);
    scenes[name] = newScene;
    return newScene;
}

void SceneManager::Delete(const std::string& name)
{
    delete Get(name);
    scenes.erase(name);
}

Scene* SceneManager::CreateDefault(const std::string& name)
{
    std::string finalName = Utils::StringUtils::Trim(name);
    if(!Utils::StringUtils::Contains(name, ".json")) {
        finalName += ".json";
    }
    const std::filesystem::path defaultScene = "Resources/Scenes/" + finalName;
    const std::string file = absolute(defaultScene).string();
    std::ofstream stream(file);
    stream << defaultSceneTemplate;
    stream.close();

    return Create(file);
}

Scene* SceneManager::Get(const std::string& name) const
{
    if (!Exists(name)) {
        DebugLogWarning("Requested scene " + name + " doesn't exist.");
        return nullptr;
    }
    return scenes.at(name);
}

void SceneManager::SetActiveScene(const std::string& name, const bool& shouldStart) const
{
    engine->GetEventDispatcher()->FrameEndDispatch(new Core::SetActiveSceneEvent(name, shouldStart));
}

void SceneManager::StartActiveScene() const
{
    if (activeScene) {
        activeScene->Start();
    }
}

void SceneManager::UpdateActiveScene(const float& deltaTime, const Core::WindowInputs& inputs) const
{
    if (activeScene)
        activeScene->Update(deltaTime, inputs);
}

void SceneManager::RenderActiveScene(const Render::Renderer* renderer, const bool& useEngineCam) const
{
    if (activeScene && activeScene->GetState() != SceneState::Unloaded)
        activeScene->Render(renderer, useEngineCam);
}

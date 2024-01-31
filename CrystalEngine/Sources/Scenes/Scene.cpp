#include "Core/Engine.h"
#include "Core/EventDispatcher.h"
#include "Core/Window.h"
#include "Core/SoundManager.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"
#include "Interface/Widget.h"

#include "Render/Camera.h"
#include "Render/CameraManager.h"
#include "Render/LightManager.h"
#include "Render/Renderer.h"
#include "Render/PostProcessor.h"

#include "Interface/Widget.h"

#include "Resources/Cubemap.h"
#include "Resources/ResourceManager.h"
#include "Resources/Font.h"
#include "Resources/Sound.h"
#include "Resources/RenderTexture.h"

#include "Physics/PhysicSystem.h"
#include "Physics/RigidbodyVolume.h"

#include "Scripts/DefaultScripts.h"
#include "Scripts/PythonScript.h"
#include "Scripts/LuaScript.h"
#include "Scripts/PlayerMovement.h"
#include "Scripts/MainMenu.h"
#include "Scripts/EnemyScript.h"
#include "Scripts/SetUpTreeScript.h"

#include "Maths/Vector3.h"

#include "Refureku/Refureku.h"

#include <LightJson/JsonObject.h>
#include <LightJson/JsonElement.h>
#include <LightJson/JsonUtils.h>
#include "Physics/Geometry3D.h"



using namespace Core;
using namespace Scenes;
using namespace Render;
using namespace Scripts;
using namespace Resources;
using namespace Physic;
using namespace LightJson;
using namespace Interface;
using namespace Maths;
using namespace Geometry3D;


Scene::Scene(std::string _filename) : filename(std::move(_filename))
{
	const Engine* engine = Engine::Get();
	const Window* window = engine->GetWindow();
    eventDispatcher = engine->GetEventDispatcher();
    resourceManager = engine->GetResourceManager();
    soundManager    = engine->GetSoundManager();

    // Create the render textures.
    renderTexture = new RenderTexture("ObjectRenderTexture_" + filename);
    renderTexture->SetSize(window->GetWidth(), window->GetHeight());
    renderTexture->Load();
    renderTexture->SendToPipeline();

    // Setup the screen resize event callback.
    Engine::Get()->GetEventDispatcher()->AddListener<ScreenResizeEvent>([this](Event* event)
    {
        const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
        if (renderTexture) renderTexture->SetSize(resizeEvent->width, resizeEvent->height);
    });
}

Scene::~Scene()
{
    Unload();
}

void Scene::Save()
{
    // Serialize all scene data.
    JsonUtils::JsonMap map = {};
    JsonUtils::JsonArray array; array.reserve(filesUsed.size());
    for (const std::string& file : filesUsed)
        array.push_back(new StringElement(file));
    
    map["Resources"]     = new ArrayElement(array);
    map["CameraManager"] = cameraManager->Serialize();
    map["LightManager"]  = lightManager ->Serialize();
    map["SceneGraph"]    = sceneGraph   ->Serialize();
    map["PostProcessor"] = Engine::Get()->GetPostProcessor()->Serialize();
    delete jsonData;
    jsonData = new JsonObject(map);

    std::function saveTask = [this]
    {
        // Save serialized data to the scene file.
        std::ofstream f(filename);
        std::ofstream fg("../CrystalGame/Resources/Scenes/" + std::filesystem::path(filename).filename().string());
        if (f.is_open())
        {
            f << JsonSerializer::toJson(jsonData);
            f.close();
        }
        if (fg.is_open())
        {
            fg << JsonSerializer::toJson(jsonData);
            fg.close();
        }
    };
    Engine::Get()->GetThreadManager()->AddTask(saveTask);
}

void Scene::Load(const bool& async)
{
    if (cameraManager || lightManager || sceneGraph || physicsSystem) Unload();
    cameraManager = new CameraManager();
    lightManager  = new LightManager();
    sceneGraph    = new SceneGraph(this);
    physicsSystem = new PhysicSystem();
    physicsSystem->ClearRigidBodys();
    physicsSystem->ClearContraints();

    std::function loadTask = [this]
    {
        // Load the scene from the json file.
        if (std::ifstream f(filename); f.is_open())
        {
            std::stringstream fString{};
            fString << f.rdbuf();
            f.close();
            delete jsonData;
            jsonData = new JsonObject(JsonDeserializer::fromJson(fString.str()));
        }

        if (jsonData)
        {
            // Load scene resources.
            const JsonUtils::JsonArray array = jsonData->getMember("Resources")->getAsArray();
            for (const JsonElement* element : array) {
                filesUsed.push_back(element->getAsString());
                resourceManager->LoadFile(filesUsed.back());
            }

            // Load cameras, lights and game objects.
            cameraManager->Deserialize(jsonData->getMember("CameraManager"));
            lightManager ->Deserialize(jsonData->getMember("LightManager" ));
            sceneGraph   ->Deserialize(jsonData->getMember("SceneGraph"   ));
            Engine::Get()->GetPostProcessor()->Deserialize(jsonData->getMember("PostProcessor"));

            state = SceneState::Loaded;
        }

        // TODO: Non-file resources should be serialized.
        Cubemap* cubemap = resourceManager->Create<Cubemap>("forest_cubemap");
        cubemap->SetCubemapSide(CubemapSide::Right,  "Resources/Textures/Skyboxes/Forest/right.png");
        cubemap->SetCubemapSide(CubemapSide::Left,   "Resources/Textures/Skyboxes/Forest/left.png");
        cubemap->SetCubemapSide(CubemapSide::Top,    "Resources/Textures/Skyboxes/Forest/top.png");
        cubemap->SetCubemapSide(CubemapSide::Bottom, "Resources/Textures/Skyboxes/Forest/bottom.png");
        cubemap->SetCubemapSide(CubemapSide::Back,   "Resources/Textures/Skyboxes/Forest/back.png");
        cubemap->SetCubemapSide(CubemapSide::Front,  "Resources/Textures/Skyboxes/Forest/front.png");
        /*
        if (GameObject* canvas = sceneGraph->Find("Canvas"))
        {
            canvas->AddScript(new MainMenu);
        }
        else
        {
            if (GameObject* camera = sceneGraph->Find("Camera"))
                camera->AddScript(new PlayerMovement);

            if (GameObject* enemy = sceneGraph->Find("SlenderMan"))
                enemy->AddScript(new EnemyScript);

            for (GameObject* object : sceneGraph->GetGameObjects())
            {
                if (object->name.find("tree") != std::string::npos)
                {
                    object->AddScript(new SetUpTreeScript);
                }
            }

            if (GameObject* floor = sceneGraph->Find("Floor1"))
            {
                floor->rigidBody = new RigidbodyVolume(floor, RIGIDBODY_TYPE_BOX);
                floor->rigidBody->position = floor->transform.GetPosition();
                floor->rigidBody->box.size = Vector3(50.f,1.0f,50.f);
                floor->rigidBody->mass = 0.0f;
                floor->rigidBody->takeGravity = false;

                floor->rigidBody->SynchCollisionVolumes();
                physicsSystem->AddRigidBody(floor->rigidBody);
            }
        }
        */
    };

    if (const Engine* engine = Engine::Get(); async && engine->GetSceneManager()->GetActiveScene() != this)
        engine->GetThreadManager()->AddTask(loadTask);
    else
        loadTask();
}

void Scene::Unload()
{
    if (state != SceneState::Unloaded)
    {
        state = SceneState::Unloaded;
        delete sceneGraph;    sceneGraph = nullptr;
        delete cameraManager; cameraManager = nullptr;
        delete lightManager;  lightManager = nullptr;
        physicsSystem->ClearContraints();
        physicsSystem->ClearRigidBodys();
        delete physicsSystem; physicsSystem = nullptr;

        for (const std::string& file : filesUsed)
            resourceManager->UnloadFile(file);
        
        filesUsed.clear();
    }
}

void Scene::Reset() const
{
    // Load cameras, lights and game floors.
    cameraManager->Deserialize(jsonData->getMember("CameraManager"));
    lightManager ->Deserialize(jsonData->getMember("LightManager" ));
    sceneGraph   ->Deserialize(jsonData->getMember("SceneGraph"   ));
}

void Scene::Start()
{
    if (state == SceneState::Loaded)
    {
        // TODO: Fix deltaTime while loading.
        Save();
        state = SceneState::Playing;
        if (const Camera* sceneCam = cameraManager->GetSceneCamera()) sceneCam->SetFullscreen(true);
        sceneGraph->StartAll();
    }
}

void Scene::Pause()
{
    if (IsPlaying())
        state = SceneState::Paused;
}

void Scene::Resume()
{
    if (IsPaused())
        state = SceneState::Playing;
}

void Scene::Stop()
{
    state = SceneState::Loaded;
    Reset();
}

void Scene::Update(const float& deltaTime, const WindowInputs& inputs) const
{
    //if (state == SceneState::Playing) physicsSystem->Update(deltaTime);
    physicsSystem->Update(deltaTime);
    cameraManager->UpdateEngineCamera(deltaTime, inputs);
    sceneGraph->UpdateAll(deltaTime, inputs, state == SceneState::Playing);



#if 0
bool colliding = OverlapOnAxis(sceneGraph->Find("Camera")->rigidBody->box, sceneGraph->Find("SlenderMan")->rigidBody->box,
       Vector3(1.0f, 0.0f, 0.0f)) && OverlapOnAxis(sceneGraph->Find("Camera")->rigidBody->box, sceneGraph->Find("SlenderMan")->rigidBody->box,
           Vector3(0.0f, 1.0f, 0.0f)) && OverlapOnAxis(sceneGraph->Find("Camera")->rigidBody->box, sceneGraph->Find("SlenderMan")->rigidBody->box,
               Vector3(0.0f, 0.0f, 1.0f));

   printf("colliding : %d\n", colliding);
#endif
}

void Scene::Render(const Renderer* renderer, const bool& useEngineCam) const
{
    Camera* camera = useEngineCam ? cameraManager->GetEngineCamera() : cameraManager->GetSceneCamera();
    if (!camera) return;

    sceneGraph   ->DrawAll(renderer, camera, lightManager);
    physicsSystem->Render (renderer, camera, lightManager);
}

void Scene::AddFile(const std::string& fileName)
{
    const auto found = std::find(filesUsed.begin(), filesUsed.end(), fileName);
    if(found == filesUsed.end()) {
        filesUsed.emplace_back(fileName);
        Save();
    }
        
}

void Scene::RemoveFile(const std::string& fileName)
{
    const auto found = std::find(filesUsed.begin(), filesUsed.end(), fileName);
    if(found != filesUsed.end()) filesUsed.erase(found);
    Save();
}

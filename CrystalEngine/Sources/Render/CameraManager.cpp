#include "Render/CameraManager.h"
#include "Core/Engine.h"
#include "Core/Window.h"
#include "Debug/Logger.h"
#include "Render/Camera.h"
#include "Maths/AngleAxis.h"
#include "Utils/JsonHelper.h"
#include <LightJson/JsonObject.h>
using namespace Render;
using namespace Maths;
using namespace LightJson;

CameraManager::CameraManager()
{
    const Core::Window* window = Core::Engine::Get()->GetWindow();
    engineCamera = new Camera(window->GetWidth(), window->GetHeight(), 0);
    engineCamera->SetFullscreen(true);
    engineCamera->transform = new Transform(true);
}

CameraManager::~CameraManager()
{
    for (const Camera* camera : cameras) {
        idGenerator.Free(camera->GetId());
        delete camera;
    }
    cameras.clear();
    delete engineCamera->transform;
    delete engineCamera;
}

Camera* CameraManager::Create(const CameraParams& params)
{
    Camera* camera = new Camera(params, idGenerator.Generate());
    cameras.emplace_back(camera);
    return camera;
}

Camera* CameraManager::Get(const size_t& id) const
{
    const auto iterator = std::find_if(cameras.begin(), cameras.end(), [id](const Camera* camera){ return camera->GetId() == id; });
    if (iterator == cameras.end()) return nullptr;
    return *iterator;
}

void CameraManager::Delete(const size_t& id)
{
    const auto iterator = std::find_if(cameras.begin(), cameras.end(), [id](const Camera* camera){ return camera->GetId() == id; });
    if (iterator == cameras.end()) {
        DebugLogWarning("Attempt to delete a camera that doesn't exist!"); return;
    }
    idGenerator.Free((*iterator)->GetId());
    delete *iterator;
    cameras.erase(iterator);
}

void CameraManager::Delete(const Camera* camera)
{
    if (const auto& iterator = std::find(cameras.begin(), cameras.end(), camera); iterator != cameras.end())
    {
        cameras.erase(iterator);
        if (sceneCamera == camera) sceneCamera = nullptr;
        delete camera;
    }
}

void CameraManager::UpdateEngineCamera(const float& deltaTime, const Core::WindowInputs& inputs) const
{
    static Core::Window* window = nullptr;
    if (!window) window = Core::Engine::Get()->GetWindow();

    window->LockHideCursor(inputs.mouseRightClick);
    if (inputs.mouseRightClick)
    {
        engineCamera->transform->Move(engineCamera->transform->GetRotation().RotateVec(inputs.movement * engineCameraSpeed * deltaTime));
        engineCamera->transform->Rotate(Quaternion::FromAngleAxis({ -inputs.mouseDelta.y * engineCameraSensitivity * 1e-3f, engineCamera->transform->Right() }));
        engineCamera->transform->Rotate(Quaternion::FromRoll     (  -inputs.mouseDelta.x * engineCameraSensitivity * 1e-3f ));
    }
}

void CameraManager::SetSceneCamera(Camera* camera)
{
    if (sceneCamera) {
        sceneCamera->SetFullscreen(false);
    }
    sceneCamera = camera;
    sceneCamera->SetFullscreen(true);
}

JsonElement* CameraManager::Serialize() const
{
    JsonUtils::JsonMap map = {};

    JsonUtils::JsonArray cameraArray = {}; cameraArray.reserve(cameras.size());
    for (const Camera* camera : cameras)
        cameraArray.push_back(camera->Serialize());
    map["cameras"]      = new ArrayElement(cameraArray);
    map["sceneCamera"]  = new NumberElement(sceneCamera ? sceneCamera->GetId() : -1);
    map["engineCamera"] = engineCamera->Serialize();
    map["engineCameraTransform"]   = Helper::JsonHelper::Serialize(*engineCamera->transform);
    map["engineCameraSpeed"]       = new NumberElement(engineCameraSpeed);
    map["engineCameraSensitivity"] = new NumberElement(engineCameraSensitivity);
    
    return new ObjectElement(map);
}

void CameraManager::Deserialize(const JsonElement* jsonElement)
{
    // Clear all cameras.
    for (const Camera* camera : cameras) {
        idGenerator.Free(camera->GetId());
        delete camera;
    }
    cameras.clear();
    delete engineCamera->transform;
    delete engineCamera;
    sceneCamera  = nullptr;
    engineCamera = nullptr;

    // Create all cameras from json data.
    const JsonUtils::JsonMap   jsonMap     = jsonElement->getAsObject();
    const JsonUtils::JsonArray cameraArray = jsonMap.at("cameras")->getAsArray();
    std::unordered_map<int, size_t> newIds{};
    for (size_t i = 0; i < cameraArray.size(); i++)
    {
        const int    oldId = cameraArray[i]->getAsObject().at("id")->getAsInteger();
        const size_t newId = idGenerator.Generate();
        newIds[oldId] = newId;
        cameras.push_back(new Camera({}, newId));
        cameras.back()->Deserialize(cameraArray[i]);
    }

    // Set the scene camera.
    if (const int oldSceneCamId = jsonMap.at("sceneCamera")->getAsInteger(); newIds.count(oldSceneCamId) > 0)
    {
        const size_t sceneCamId = newIds.at(oldSceneCamId);
        const auto iterator = std::find_if(cameras.begin(), cameras.end(), [sceneCamId](const Camera* cam){ return cam->GetId() == sceneCamId; });
        if (iterator != cameras.end())
            SetSceneCamera(*iterator);
    }

    // Set the engine camera.
    engineCamera = new Camera({}, 0);
    engineCamera->Deserialize(jsonMap.at("engineCamera"));
    engineCamera->transform = new Transform(true);
   *engineCamera->transform = Helper::JsonHelper::Deserialize<Transform>(jsonMap.at("engineCameraTransform"));
    engineCamera->SetFullscreen(true);
    engineCameraSpeed       = jsonMap.at("engineCameraSpeed")->getAsFloat();
    engineCameraSensitivity = jsonMap.at("engineCameraSensitivity")->getAsFloat();
}

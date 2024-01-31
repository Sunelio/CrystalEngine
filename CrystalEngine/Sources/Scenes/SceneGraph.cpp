#include <algorithm>
#include "Utils/Typedefs.h"
#include "Core/Engine.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"
#include "Render/Camera.h"
#include "Render/Light.h"
#include "Utils/IdGenerator.h"
#include "LightJson/JsonUtils.h"
#include "LightJson/JsonElement.h"
#include "LightJson/JsonObject.h"
#include "Render/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Scenes/Scene.h"
#include "Utils/JsonHelper.h"
using namespace LightJson;
using namespace Resources;
using namespace Scenes;
using namespace Utils;
using namespace Core;

SceneGraph::SceneGraph(Scene* _scene) : root(new GameObject(_scene, 0, "Root", nullptr)), scene(_scene)
{
    // Add event listener to remove game objects.
    Engine::Get()->GetEventDispatcher()->AddListener<RemoveGameObjectEvent>([this](Event* event)
    {
        RemoveGameObjectEvent* removeEvent = (RemoveGameObjectEvent*)event;
        const auto iterator = std::find_if(gameObjects.begin(), gameObjects.end(), [id=removeEvent->id](const GameObject* object){ return object->GetId() == id; });
        if (iterator == gameObjects.end()) return;
        const GameObject* object = *iterator;
        if (object->GetParent()) object->GetParent()->RemoveChild(object->GetId());
        delete object;
    });
}

SceneGraph::~SceneGraph()
{
    idGenerator.Clear();
    delete root;
    root = nullptr;
}

GameObject* SceneGraph::AddEmpty(const std::string& name, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    GameObject* newObject = new GameObject(scene, idGenerator.Generate(), name, parent);
    parent->AddChild(newObject);
    gameObjects.emplace_back(newObject);
    return newObject;
}

ModelObject* SceneGraph::AddModel(const std::string& name, Mesh* mesh, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    ModelObject* newObject = new ModelObject(scene, idGenerator.Generate(), name, mesh, parent);
    parent->AddChild(newObject);
    gameObjects    .emplace_back(newObject);
    drawableObjects.emplace_back(newObject);
    return newObject;
}

CubemapObject* SceneGraph::AddCubemap(const std::string& name, Cubemap* cubemap, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    CubemapObject* newObject = new CubemapObject(scene, idGenerator.Generate(), name, cubemap, parent);
    parent->AddChild(newObject);
    gameObjects    .emplace_back(newObject);
    drawableObjects.emplace_back(newObject);
    return newObject;
}

InstancedObject* SceneGraph::AddInstanced(const std::string& name, Mesh* mesh, const int& instanceCount, GameObject* parent)
{
    // Set the parent if it is null.
     if (parent == nullptr)
         parent = root;
 
     // Create a new object and add it to the parent's children.
     InstancedObject* newObject = new InstancedObject(scene, idGenerator.Generate(), name, mesh, instanceCount, parent);
     parent->AddChild(newObject);
     gameObjects    .emplace_back(newObject);
     drawableObjects.emplace_back(newObject);
     return newObject;
}

LightObject* SceneGraph::AddLight(const std::string& name, Render::Light* light, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    LightObject* newObject = new LightObject(scene, idGenerator.Generate(), name, light, parent);
    parent->AddChild(newObject);
    gameObjects.emplace_back(newObject);
    return newObject;
}

CameraObject* SceneGraph::AddCamera(const std::string& name, Render::Camera* camera, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    CameraObject* newObject = new CameraObject(scene, idGenerator.Generate(), name, camera, parent);
    parent->AddChild(newObject);
    gameObjects.emplace_back(newObject);
    return newObject;
}

SoundObject* SceneGraph::AddSound(const std::string& name, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;
 
    // Create a new object and add it to the parent's children.
    SoundObject* newObject = new SoundObject(scene, idGenerator.Generate(), name, parent);
    parent->AddChild(newObject);
    gameObjects.emplace_back(newObject);
    return newObject;
}

CanvasObject* SceneGraph::AddCanvas(const std::string& name, GameObject* parent)
{
    // Set the parent if it is null.
    if (parent == nullptr)
        parent = root;

    // Create a new object and add it to the parent's children.
    CanvasObject* newObject = new CanvasObject(scene, idGenerator.Generate(), name, parent);
    parent->AddChild(newObject);
    gameObjects.emplace_back(newObject);
    interfaceObjects.emplace_back(newObject);
    return newObject;
}

void SceneGraph::Remove(const GameObject* gameObject)
{
    Engine::Get()->GetEventDispatcher()->FrameEndDispatch(new RemoveGameObjectEvent(gameObject->GetId()));
}

void SceneGraph::Remove(const size_t& id)
{
    Engine::Get()->GetEventDispatcher()->FrameEndDispatch(new RemoveGameObjectEvent(id));
}

ObjectTag* SceneGraph::CreateTag(const std::string& name)
{
    ObjectTag* tag = new ObjectTag(name);
    objectTags[name] = tag;
    return tag;
}

void SceneGraph::StartAll()
{
    for (const GameObject* gameObject : gameObjects) {
        gameObject->PlayModeStart();
    }

    for (const GameObject* gameObject : gameObjects) {
        gameObject->PlayModeLateStart();
    }
}

void SceneGraph::UpdateAll(const float& deltaTime, const WindowInputs& inputs, const bool& updateScripts) const
{
    if (root) root->RecursiveUpdate(deltaTime, inputs, updateScripts);
}

void SceneGraph::DrawAll(const Render::Renderer* renderer, const Render::Camera* camera, const Render::LightManager* lightManager) const
{
    if (!drawableObjects.empty())
    {
        for (size_t i = 0; i < drawableObjects.size(); i++)
        {
            switch (drawableObjects[i]->GetType())
            {
            case GameObjectType::Model:
                ((const ModelObject*)drawableObjects[i])->Draw(renderer, camera, lightManager);
                break;
            case GameObjectType::Instanced:
                ((InstancedObject*)drawableObjects[i])->Draw(renderer, camera, lightManager);
                break;
            case GameObjectType::Cubemap:
                ((CubemapObject*)drawableObjects[i])->Draw(renderer, camera, lightManager);
                break;
            default:
                break;
            }
        }
    }
    if (!interfaceObjects.empty())
    {
        for (size_t i = 0; i < interfaceObjects.size(); i++)
        {
            switch (interfaceObjects[i]->GetType())
            {
            case GameObjectType::Canvas:
                ((CanvasObject*)interfaceObjects[i])->Draw(renderer);
                break;
            default:
                break;
            }
        }
    }
}

void SceneGraph::ClearAll()
{
    for(const GameObject* gameObject : gameObjects) {
        idGenerator.Free(gameObject->GetId());
    }
    gameObjects.clear();
    drawableObjects.clear();
    
    delete root;
    root = new GameObject(scene, 0, "Root", nullptr);
}

GameObject* SceneGraph::Find(const size_t& searchId) const
{
    const auto iterator = std::find_if(gameObjects.begin(), gameObjects.end(), [searchId](const GameObject* gameObject){ return gameObject->GetId() == searchId; });
    if (iterator == gameObjects.end()) return nullptr;
    return *iterator;
}

GameObject* SceneGraph::Find(const std::string& searchName) const
{
    const auto iterator = std::find_if(gameObjects.begin(), gameObjects.end(), [searchName](const GameObject* gameObject){ return gameObject->name == searchName; });
    if (iterator == gameObjects.end()) return nullptr;
    return *iterator;
}

GameObject* SceneGraph::FindWithTag(const std::string& searchName, const std::string& tag)
{
    if (objectTags.count(tag) <= 0) return nullptr;
    std::vector<GameObject*>& tagged = objectTags[tag]->tagged;
    const auto iterator = std::find_if(tagged.begin(), tagged.end(), [searchName](const GameObject* gameObject){ return gameObject->name == searchName; });
    if (iterator == tagged.end()) return nullptr;
    return *iterator;
}

JsonElement* SceneGraph::Serialize() const
{
    JsonUtils::JsonMap map = {};

    // Serialize root ID.
    map["rootObject"] = new NumberElement(root->GetId());
    
    // Serialize game objects.
    JsonUtils::JsonArray objectsArray {};
    objectsArray.reserve(gameObjects.size());
    for (const GameObject* gameObject : gameObjects) {
        if(gameObject->GetDestroyState()) continue;
        objectsArray.emplace_back(gameObject->Serialize());
    }
    map["gameObjects"] = new ArrayElement(objectsArray);

    // Serialize tags.
    JsonUtils::JsonArray tagsArray {};
    tagsArray.reserve(objectTags.size());
    for(const auto& [key, value] : objectTags) {
        tagsArray.emplace_back(Helper::JsonHelper::Serialize<ObjectTag>(*value));
    }
    map["objectTags"] = new ArrayElement(tagsArray);
    
    return new ObjectElement(map);
}

void SceneGraph::Deserialize(const JsonElement* jsonElement)
{
    ClearAll();
    const JsonUtils::JsonMap   jsonMap       = jsonElement->getAsObject();
    const JsonUtils::JsonArray objectsArray  = jsonMap.at("gameObjects"    )->getAsArray();
    const JsonUtils::JsonArray tagsArray     = jsonMap.at("objectTags"     )->getAsArray();
    std::unordered_map<int, size_t> newIds{};

    // Deserialize game objects data.
    for (const JsonElement* element : objectsArray)
    {
        const JsonUtils::JsonMap map = element->getAsObject();
        const GameObjectType objectType = GameObjectTypeFromString(map.at("type")->getAsString());
        const std::string    objectName = map.at("name")->getAsString();
        
        GameObject* gameObject = nullptr;
        switch (objectType)
        {
        case GameObjectType::Empty:
            gameObject = AddEmpty(objectName);
            break;
        case GameObjectType::Model:
            gameObject = AddModel(objectName, nullptr);
            break;
        case GameObjectType::Instanced:
            gameObject = AddInstanced(objectName, nullptr, map.at("instanceCount")->getAsInteger());
            break;
        case GameObjectType::Camera:
            gameObject = AddCamera(objectName, nullptr);
            break;
        case GameObjectType::Cubemap:
            gameObject = AddCubemap(objectName, nullptr);
            break;
        case GameObjectType::Canvas:
            gameObject = AddCanvas(objectName);
            break;
        case GameObjectType::Light:
            gameObject = AddLight(objectName, nullptr);
            break;
        case GameObjectType::Sound:
            gameObject = AddSound(objectName, nullptr);
            break;
        default:
            break;
        }
        if (!gameObject) continue;
        gameObject->Deserialize(element);
        newIds[map.at("id")->getAsInteger()] = gameObject->GetId();
    }

    // Deserialize hierarchy.
    const int rootId = jsonMap.at("rootObject")->getAsInteger();
    for (size_t i = 0; i < gameObjects.size() && i < objectsArray.size(); i++)
    {
        const int parentId = objectsArray[i]->getAsObject().at("parent")->getAsInteger();
        if (rootId != parentId)
            gameObjects[i]->SetParent(Find(newIds[parentId]));
        else
            gameObjects[i]->SetParent(root);
    }

    // Deserialize tags.
    for (const JsonElement* tagElement : tagsArray)
    {
        const JsonUtils::JsonMap   map   = tagElement->getAsObject();
        const JsonUtils::JsonArray array = map.at("tagged")->getAsArray();
        
        ObjectTag* tag = CreateTag(map.at("name")->getAsString());
        for (JsonElement* taggedElement : array)
            tag->Tag(Find(newIds[taggedElement->getAsInteger()]));
    }
}
    

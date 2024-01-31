#include "Utils/Typedefs.h"
#include "Utils/IdGenerator.h"
#include "Utils/JsonHelper.h"

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/SoundManager.h"

#include "Render/Renderer.h"
#include "Render/Camera.h"
#include "Render/CameraManager.h"
#include "Render/Light.h"
#include "Render/LightManager.h"

#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Resources/Cubemap.h"

#include "Resources/ResourceManager.h"

#include "Scenes/GameObject.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/SceneManager.h"

#include "Interface/Widget.h"

#include "Animation/Skeleton.h"

#include "Physics/RigidbodyVolume.h"

#include <LightJson/JsonObject.h>

#include "Scripts/ObjectScript.h"
#include "Scripts/PythonScript.h"
#include "Scripts/LuaScript.h"

using namespace Scenes;
using namespace Interface;
using namespace Render;
using namespace Resources;
using namespace Scripts;
using namespace Maths;
using namespace LightJson;
using namespace Physic;

std::string Scenes::GameObjectTypeToString(const GameObjectType& type)
{
    switch (type)
    {
    case GameObjectType::Model:
        return "Model";
    case GameObjectType::Instanced:
        return "Instanced";
    case GameObjectType::Camera:
        return "Camera";
    case GameObjectType::Cubemap:
        return "Cubemap";
    case GameObjectType::Canvas:
        return "Canvas";
    case GameObjectType::Light:
        return "Light";
    case GameObjectType::Sound:
        return "Sound";
    case GameObjectType::Empty:
    default:
        return "Empty";
    }
}

GameObjectType Scenes::GameObjectTypeFromString(const std::string& type)
{
    if (type == "Model")
        return GameObjectType::Model;
    if (type ==  "Instanced")
        return GameObjectType::Instanced;
    if (type ==  "Camera")
        return GameObjectType::Camera;
    if (type ==  "Cubemap")
        return GameObjectType::Cubemap;
    if (type ==  "Canvas")
        return GameObjectType::Canvas;
    if (type ==  "Light")
        return GameObjectType::Light;
    if (type ==  "Sound")
        return GameObjectType::Sound;
    return GameObjectType::Empty;
}


GameObject::GameObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent, const GameObjectType& _type)
{
    scene  = _scene;
    id     = _id;
    name   = _name;
    parent = _parent;
    type   = _type;
    if (parent) transform.SetParentMat(_parent->transform.GetWorldMat());
}

GameObject::~GameObject()
{
    // Delete scripts and children.
    for (const ObjectScript* script : scripts ) delete script;
    for (const GameObject*   child  : children) delete child;
    scripts .clear();
    children.clear();
    
    // Remove self from scene graph.
    SceneGraph* sceneGraph = scene->GetSceneGraph();
    const auto iterator1 = std::find(sceneGraph->gameObjects     .begin(), sceneGraph->gameObjects     .end(), this);
    const auto iterator2 = std::find(sceneGraph->drawableObjects .begin(), sceneGraph->drawableObjects .end(), this);
    const auto iterator3 = std::find(sceneGraph->interfaceObjects.begin(), sceneGraph->interfaceObjects.end(), this);
    if (iterator1 != sceneGraph->gameObjects     .end()) sceneGraph->gameObjects     .erase(iterator1);
    if (iterator2 != sceneGraph->drawableObjects .end()) sceneGraph->drawableObjects .erase(iterator2);
    if (iterator3 != sceneGraph->interfaceObjects.end()) sceneGraph->interfaceObjects.erase(iterator3);
    sceneGraph->idGenerator.Free(id);
}

void GameObject::PlayModeStart() const
{
    for (ObjectScript* script : scripts)
        script->Start();
}

void GameObject::PlayModeLateStart() const
{
    for (ObjectScript* script : scripts)
        script->LateStart();
}

void GameObject::RecursiveUpdate(const float& deltaTime, const Core::WindowInputs& inputs, const bool& updateScripts)
{
    if (rigidBody && rigidBody->mass != 0.0f && updateScripts)
    {
        transform.SetPosition(rigidBody->position);
    }


    // Update skeletons from animated meshes.
    // TODO: This needs to be done once per animator in the scene.
    if (type == GameObjectType::Model)
    {
        const ModelObject* model = (const ModelObject*)this;
        if (Anim::Animator* animator = model->mesh->GetAnimator())
            animator->Update(deltaTime);
    }

    // Update canvas and widgets.
    if (type == GameObjectType::Canvas)
        ((CanvasObject*)this)->Update(inputs);

    // Call script updates.
    if (updateScripts) {
        for (ObjectScript* script : scripts) {
            script->inputs = inputs;
            script->Update();
        }
    }
    
    // Update all children.
    const Mat4 curWorldMat = transform.GetWorldMat();
    for (GameObject* child : children) {
        child->transform.SetParentMat(curWorldMat);
        child->RecursiveUpdate(deltaTime, inputs, updateScripts);
    }
    
    // Call script late updates.
    if (updateScripts) {
        for (ObjectScript* script : scripts) {
            script->inputs = inputs;
            script->LateUpdate();
        }
    }
}

void GameObject::AddScript(ObjectScript* script)
{
    script->engine    = Core::Engine::Get();
    script->scene     = scene;
    script->time      = script->engine->GetTimeManager();
    script->object    = this;
    script->transform = &transform;
    scripts.push_back(script);

    if (scene->GetState() == SceneState::Playing || scene->GetState() == SceneState::Paused) 
    {
        script->Start();
        script->LateStart();
    }
}

ObjectScript* GameObject::GetScript(const size_t& idx) const
{
    if (idx >= scripts.size())
        return nullptr;
    return scripts[idx];
}

void GameObject::RemoveScript(const size_t& idx)
{
    if (idx >= scripts.size())
        return;
    delete scripts[idx];
    scripts.erase(scripts.begin() + idx);
}

void GameObject::RemoveScript(const ObjectScript* script)
{
    const auto idx = std::find(scripts.begin(), scripts.end(), script);
    if (idx == scripts.end())
        return;
    delete *idx;
    scripts.erase(idx);
}

void GameObject::AddChild(GameObject* child)
{
    children.push_back(child);
}

void GameObject::RemoveChild(const size_t& childId)
{
    const auto iterator = std::find_if(children.begin(), children.end(), [childId](const GameObject* child){ return child->GetId() == childId; });
    if (iterator == children.end()) return;
    children.erase(iterator);
}

bool GameObject::SetParent(GameObject* newParent)
{
    if (!FindInChildren(newParent->GetId()))
    {
        if (parent) parent->RemoveChild(id);
        parent = newParent;
        transform.SetParentMat(newParent->transform.GetWorldMat());
        newParent->children.push_back(this);
        return true;
    }
    return false;
}

GameObject* GameObject::FindInChildren(const size_t& searchId) const
{
    // Find in direct children.
    for (GameObject* child : children)
        if (child->id == searchId)
            return child;

    // Find in children's children.
    for (const GameObject* child : children)  {
        GameObject* foundChild = child->FindInChildren(searchId);
        if (foundChild != nullptr)
            return foundChild;
    }
    return nullptr;
}

GameObject* GameObject::FindInChildren(const std::string& searchName) const
{
    // Find in direct children.
    for (GameObject* child : children)
        if (child->name == searchName)
            return child;

    // Find in children's children.
    for (const GameObject* child : children)  {
        GameObject* foundChild = child->FindInChildren(searchName);
        if (foundChild != nullptr)
            return foundChild;
    }
    return nullptr;
}

JsonElement* GameObject::Serialize() const
{
    JsonUtils::JsonMap map{};
    map["id"]        = new NumberElement(id);
    map["name"]      = new StringElement(name);
    map["type"]      = new StringElement(GameObjectTypeToString(type));
    map["parent"]    = new NumberElement(parent ? (int)parent->id : -1);
    map["transform"] = Helper::JsonHelper::Serialize(transform);
    
    JsonUtils::JsonArray array{}; array.reserve(scripts.size());/*
    for (const ObjectScript* script : scripts)
    {
        JsonUtils::JsonMap scriptData{};
        scriptData["id"] = new StringElement(std::to_string(script->GetArchId()));
        if (script->GetArchId() == rfk::getDatabase().getNamespaceByName("Scripts")->getStructByName("PythonScript")->getId())
        {
            scriptData["filename"] = new StringElement(((const PythonScript*)script)->GetName());
        }
        else if (script->GetArchId() == rfk::getDatabase().getNamespaceByName("Scripts")->getStructByName("LuaScript")->getId())
        {
            scriptData["filename"] = new StringElement(((const LuaScript*)script)->GetName());
        }
        array.push_back(new ObjectElement(scriptData));
    }*/
    map["scripts"] = new ArrayElement(array);
    
    
    return new ObjectElement(map);
}

void GameObject::Deserialize(const JsonElement* jsonElement)
{
    JsonUtils::JsonMap map = jsonElement->getAsObject();
    transform = Helper::JsonHelper::Deserialize<Transform>(map["transform"]);
/*
    const JsonUtils::JsonArray array = map["scripts"]->getAsArray();
    scripts.reserve(array.size());
    for (const JsonElement* element : array)
    {
        std::string archIdStr = element->getAsObject()["id"]->getAsString();
        std::stringstream sstream(archIdStr);
        size_t archId = 0; sstream >> archId;
        const rfk::Struct* scriptType = rfk::getDatabase().getStructById(archId);
        if (!scriptType) continue; 
        ObjectScript* script;
        if (archId == rfk::getDatabase().getNamespaceByName("Scripts")->getStructByName("PythonScript")->getId() ||
            archId == rfk::getDatabase().getNamespaceByName("Scripts")->getStructByName("LuaScript")   ->getId())
        {
            const std::string filename = element->getAsObject()["filename"]->getAsString();
            rfk::UniquePtr<ObjectScript> scriptPtr = scriptType->makeUniqueInstance<ObjectScript>(filename);
            script = scriptPtr.release();
        }
        else
        {
            rfk::UniquePtr<ObjectScript> scriptPtr = scriptType->makeUniqueInstance<ObjectScript>();
            script = scriptPtr.release();
        }
        AddScript(script);
    }
*/
}

ModelObject::ModelObject(Scene* _scene, const size_t& _id, const std::string& _name, Mesh* _mesh, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Model)
{
    mesh = _mesh;
}

void ModelObject::Draw(const Renderer* renderer, const Camera* camera, const LightManager* lightManager) const
{
    if (mesh != nullptr)
    {
        const Mat4 worldMat = transform.GetWorldMat();
        for (size_t i = 0; i < mesh->subMeshes.size(); i++)
        {
            if (!mesh->subMeshes[i]->WasSentToPipeline())
                continue;
            renderer->DrawSubMesh(mesh->subMeshes[i], worldMat, camera, lightManager, id);
        }
    }
}

JsonElement* ModelObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    element->addElement("mesh", new StringElement(mesh->GetName()));
    return element;
}

void ModelObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
    
    JsonUtils::JsonMap map      = jsonElement->getAsObject();
    const  std::string meshName = map["mesh"]->getAsString();
    mesh = Core::Engine::Get()->GetResourceManager()->Get<Mesh>(meshName, true);
}

InstancedObject::InstancedObject(Scene* _scene, const size_t& _id, const std::string& _name, Mesh* _mesh, const size_t& _instanceCount, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Instanced)
{
    mesh          = _mesh;
    instanceCount = _instanceCount;
    transforms.resize(instanceCount, Transform());

    for (Transform& t : transforms) {
        t.SetPosition(Vector3(0, (float)(rand() % 4 - 2), 0) + Vector3(Vector3(0, degToRad((float)(rand() % 360)), 0), (float)(rand() % 20 + 25)));
        t.SetRotation(Vector3(((float)(rand() % 5)) / 100.f));
    }

    if (mesh && !mesh->IsInstanced()) mesh->MakeInstanced();
}

void InstancedObject::Draw(const Renderer* renderer, const Camera* camera, const LightManager* lightManager)
{
    if (!mesh) return;
    if (!mesh->IsInstanced()) { mesh->MakeInstanced(); return; }
    
    // TODO: Only update matrices and buffer when one is modified.
    std::vector<Mat4> matrices;
    matrices.reserve(instanceCount);
    for (Transform& t : transforms)
        matrices.push_back(t.GetLocalMat());
    renderer->SetShaderArrayBufferData(mesh->GetInstanceMatBuffer(), instanceCount * sizeof(Mat4), matrices.data());
     
    const Mat4 worldMat = transform.GetWorldMat();
    for (const SubMesh* subMesh : mesh->subMeshes)
    {
        if (!subMesh->WasSentToPipeline()) continue;
        renderer->DrawInstanced(subMesh, (int)instanceCount, worldMat, camera, lightManager, id);
    }
}

JsonElement* InstancedObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    element->addElement("mesh",          new StringElement(mesh->GetName()));
    element->addElement("instanceCount", new NumberElement(instanceCount));
    /*
    JsonUtils::JsonArray transformArray = {}; transformArray.reserve(transforms.size());
    for (size_t i = 0; i < transforms.size(); i++) {
        transformArray.emplace_back(Helper::JsonHelper::Serialize(transforms[i]));
    }
    element->addElement("transforms", new ArrayElement(transformArray));
    */
    
    return element;
}

void InstancedObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
    JsonUtils::JsonMap map = jsonElement->getAsObject();
    
    const std::string meshName = map["mesh"]->getAsString();
    mesh = Core::Engine::Get()->GetResourceManager()->Get<Mesh>(meshName, true);
    
    instanceCount = map["instanceCount"]->getAsInteger();
    transforms.resize(instanceCount, Transform());
    for (Transform& t : transforms) {
        t.SetPosition(Vector3(0, (float)(rand() % 4 - 2), 0) + Vector3(Vector3(0, degToRad((float)(rand() % 360)), 0), (float)(rand() % 20 + 25)));
        t.SetRotation(Vector3(((float)(rand() % 5)) / 100.f));
    }
}

CameraObject::CameraObject(Scene* _scene, const size_t& _id, const std::string& _name, Camera* _camera, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Camera)
{
    camera = _camera;
    if (camera) {
        camera->transform = &transform;
        camera->transform->SetIsCamera(true);
    }
}

JsonElement* CameraObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    if (camera) element->addElement("camera", new NumberElement(camera->GetId()));
    return element;
}

void CameraObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);

    JsonUtils::JsonMap map = jsonElement->getAsObject();
    const int cameraId = map["camera"]->getAsInteger();
    camera = scene->GetCameraManager()->Get(cameraId);
    if (camera) {
        camera->transform = &transform;
        camera->transform->SetIsCamera(true);
    }
}

LightObject::LightObject(Scene* _scene, const size_t& _id, const std::string& _name, Light* _light, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Light)
{
    light = _light;
    if (light) light->transform = &transform;
}

JsonElement* LightObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    element->addElement("light", new NumberElement(light->GetId()));
    return element;
}

void LightObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
    
    JsonUtils::JsonMap map = jsonElement->getAsObject();
    const int lightId = map["light"]->getAsInteger();
    light = scene->GetLightManager()->Get(lightId);
    if (light) light->transform = &transform;
}

CubemapObject::CubemapObject(Scene* _scene, const size_t& _id, const std::string& _name, Cubemap* _cubemap, GameObject* _parent):
    GameObject(_scene, _id, _name, _parent, GameObjectType::Cubemap)
{
    cubemap = _cubemap;
    mesh = Core::Engine::Get()->GetResourceManager()->Get<Mesh>("mesh_cubemap");
}

void CubemapObject::Draw(const Renderer* renderer, const Camera* camera, const LightManager* lightManager) const
{
    if (mesh != nullptr)
    {
        const Mat4 worldMat = transform.GetWorldMat();
        for (size_t i = 0; i < mesh->subMeshes.size(); i++)
        {
            if (!mesh->subMeshes[i]->WasSentToPipeline())
                continue;
            
            renderer->DrawCubemap(mesh->subMeshes[i], cubemap, worldMat, camera, lightManager, id);
        }
    }
}

JsonElement* CubemapObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    element->addElement("cubemap", new StringElement(cubemap->GetName()));
    return element;
}

void CubemapObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
    JsonUtils::JsonMap map = jsonElement->getAsObject();

    const std::string cubemapName = map["cubemap"]->getAsString();
    cubemap = Core::Engine::Get()->GetResourceManager()->Get<Cubemap>(cubemapName, true);
}

CanvasObject::CanvasObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Canvas)
{
    rootWidget = new Widget(this, 0, "Root");
}

CanvasObject::~CanvasObject()
{
    //NOTE(o.luanda):The constructor of Widget already destroy widget
#if 0
    for (const Widget* widget : widgets)
    {
        if (!widget) continue;
        delete widget;
    }
    widgets.clear();
#endif
}

void CanvasObject::Update(const Core::WindowInputs& inputs) const
{
    for (Widget* widget : widgets)
        widget->Update(inputs);
}

void CanvasObject::Draw(const Renderer* renderer) const
{
    for (Widget* widget : widgets)
        widget->Draw(renderer);
}

Widget* CanvasObject::AddEmptyWidget(const std::string& widgetName, Widget* widgetParent)
{
    // Set the parent if it is null.
    if (widgetParent == nullptr)
        widgetParent = rootWidget;

    // Create a new widget and add it to the parent's children.
    Widget* newWidget = new Widget(this, idGenerator.Generate(), widgetName);
    newWidget->SetParent(widgetParent);
    widgets.emplace_back(newWidget);
    return newWidget;
}

TextWidget* CanvasObject::AddTextWidget(const std::string& widgetName, Widget* widgetParent)
{
    // Set the parent if it is null.
    if (widgetParent == nullptr)
        widgetParent = rootWidget;

    // Create a new widget and add it to the parent's children.
    TextWidget* newWidget = new TextWidget(this, idGenerator.Generate(), widgetName);
    newWidget->SetParent(widgetParent);
    widgets.emplace_back(newWidget);
    return newWidget;
}

ButtonWidget* CanvasObject::AddButtonWidget(const std::string& widgetName, Widget* widgetParent)
{
    // Set the parent if it is null.
    if (widgetParent == nullptr)
        widgetParent = rootWidget;

    // Create a new widget and add it to the parent's children.
    ButtonWidget* newWidget = new ButtonWidget(this, idGenerator.Generate(), widgetName);
    newWidget->SetParent(widgetParent);
    widgets.emplace_back(newWidget);
    return newWidget;
}

void CanvasObject::RemoveWidget(const Widget* widget)
{
    Core::Engine::Get()->GetEventDispatcher()->Dispatch(new Core::RemoveWidgetEvent(widget->GetId()));
    const auto iterator = std::find(widgets.begin(), widgets.end(), widget);
    if (iterator == widgets.end()) return;
    widget->GetParent()->RemoveChild(widget->GetId());
    delete widget;
}

void CanvasObject::RemoveWidget(const size_t& widgetId)
{
    const auto iterator = std::find_if(widgets.begin(), widgets.end(), [widgetId](const Widget* widget){ return widget->GetId() == widgetId; });
    if (iterator == widgets.end()) return;
    const Widget* widget = *iterator;
    widget->GetParent()->RemoveChild(widget->GetId());
    delete widget;
}

Widget* CanvasObject::FindWidget(const size_t& widgetId)
{
    const auto it = std::find_if(widgets.begin(), widgets.end(), [widgetId](const Widget* widget) { return widget->GetId() == widgetId; });
    if (it == widgets.end()) return nullptr;
    return *it;
}

Widget* CanvasObject::FindWidget(const std::string& widgetname)
{
    const auto it = std::find_if(widgets.begin(), widgets.end(), [widgetname](const Widget* widget) { return widget->GetName() == widgetname; });
    if (it == widgets.end()) return nullptr;
    return *it;
}

JsonElement* CanvasObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    JsonUtils::JsonArray widgetsArray{}; widgetsArray.reserve(widgets.size());
    for (const Widget* widget : widgets)
        widgetsArray.push_back(widget->Serialize());
    element->addElement("widgets", new ArrayElement(widgetsArray));
    return element;
}

void CanvasObject::Deserialize(const JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
    const JsonUtils::JsonMap   jsonMap      = jsonElement->getAsObject();
    const JsonUtils::JsonArray widgetsArray = jsonMap.at("widgets")->getAsArray();
    widgets.reserve(widgetsArray.size());
    std::unordered_map<int, size_t> newIds{};

    // Deserialize widget data.
    for (const JsonElement* widgetElement : widgetsArray)
    {
        const JsonUtils::JsonMap widgetData = widgetElement->getAsObject();
        const WidgetType  widgetType = (WidgetType)widgetData.at("type")->getAsInteger();
        const std::string widgetName = widgetData.at("name")->getAsString();
        
        Widget* widget = nullptr;
        switch (widgetType)
        {
        case WidgetType::Text:
            widget = AddTextWidget(widgetName);
            break;
        case WidgetType::Button:
            widget = AddButtonWidget(widgetName);
            break;
        case WidgetType::Empty:
        default:
            break;
        }
        if (!widget) continue;
        widget->Deserialize(widgetElement);
        newIds[jsonMap.at("id")->getAsInteger()] = widget->GetId();
    }
/*
    // Deserialize hierarchy.
    for (size_t i = 0; i < widgets.size(); i++)
    {
        const int parentId = widgetsArray[i]->getAsObject().at("parent")->getAsInteger();
        if (parentId)
            widgets[i]->SetParent(FindWidget(newIds[parentId]));
        else
            widgets[i]->SetParent(widgetRoot);
    }*/
}

SoundObject::SoundObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent)
    : GameObject(_scene, _id, _name, _parent, GameObjectType::Sound) { }

void SoundObject::AddSound(const std::string& filename)
{
    Sound* sound = Core::Engine::Get()->GetResourceManager()->Get<Sound>(filename);
    if (!sound) return;

    sounds.emplace_back(sound);
}

void SoundObject::PlayCurrentSound()
{
    if (!currentSound) return;
    currentSound->Start();
}

void SoundObject::SetCurrentSound(Sound* sound)
{
    currentSound = sound;
    if (currentSound == nullptr) return;

    Render::Camera* player = Core::Engine::Get()->GetSceneManager()->GetActiveScene()->GetCameraManager()->GetSceneCamera();
    const Core::SoundManager* soundManager = currentSound->GetSoundManager();

    Maths::Vector3 position = player->transform->GetPosition();
    soundManager->SetListenerPosition (0, &position);

    Maths::Vector3 direction = player->transform->Forward();
    soundManager->SetListenerDirection(0, &direction);
}

JsonElement* SoundObject::Serialize() const
{
    ObjectElement* element = (ObjectElement*)GameObject::Serialize();
    JsonUtils::JsonArray soundArray{}; soundArray.reserve(sounds.size());
    for (const Sound* sound : sounds)
        soundArray.push_back(Helper::JsonHelper::Serialize(*sound));
    element->addElement("sounds", new ArrayElement(soundArray));
    return element;
}

void SoundObject::Deserialize(const LightJson::JsonElement* jsonElement)
{
    GameObject::Deserialize(jsonElement);
}

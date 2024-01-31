#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "ObjectTag.h"
#include "Utils/IdGenerator.h"

namespace Scenes
{
    class InstancedObject;
}

namespace Scenes
{
    struct ObjectTag;
}

namespace Render
{
    class LightManager;
    class Light;
    class Camera;
}
namespace Core      { struct WindowInputs; }
namespace Resources { class Mesh; }
namespace Scenes
{
    class Scene;
    class GameObject;
    class ModelObject;
    class AnimatedObject;
    class LightObject;
    class CameraObject;

    // - SceneGraph - //
    class SceneGraph : public LightJson::JsonVirtual
    {
    private:
        std::unordered_map<std::string, ObjectTag*> objectTags;
        std::vector<GameObject*> gameObjects;
        std::vector<GameObject*> drawableObjects;
        std::vector<GameObject*> interfaceObjects;
        GameObject* root;

        Utils::IdGenerator idGenerator;
        Scene* scene = nullptr;
        friend GameObject;
        
    public:
        SceneGraph(Scene* _scene);
        ~SceneGraph() override;
        SceneGraph(const SceneGraph&)            = delete;
        SceneGraph(SceneGraph&&)                 = delete;
        SceneGraph& operator=(const SceneGraph&) = delete;
        SceneGraph& operator=(SceneGraph&&)      = delete;

        // -- Handling game objects -- //
        GameObject*      AddEmpty    (const std::string& name,                              GameObject* parent = nullptr); // Creates a new empty game object with the given name.
        ModelObject*     AddModel    (const std::string& name, Resources::Mesh* mesh,       GameObject* parent = nullptr); // Creates a new model object with the given name and mesh.
        InstancedObject* AddInstanced(const std::string& name, Resources::Mesh* mesh, const int& instanceCount, GameObject* parent = nullptr); // Creates a new instanced model object with the given name and mesh.
        LightObject*     AddLight    (const std::string& name, Render::Light*   light,      GameObject* parent = nullptr); // Creates a new light object with the given name and light.
        CameraObject*    AddCamera   (const std::string& name, Render::Camera*  camera,     GameObject* parent = nullptr); // Creates a new camera object with the given name and camera.
        CubemapObject*   AddCubemap  (const std::string& name, Resources::Cubemap* cubemap, GameObject* parent = nullptr); // Creates a new cubemap object with the given name and mesh.
        SoundObject*     AddSound    (const std::string& name,                              GameObject* parent = nullptr); // Creates a new sound object with the given name.
        CanvasObject*    AddCanvas   (const std::string& name,                              GameObject* parent = nullptr); // Creates a new canvas object with the given name and mesh.
        void             Remove      (const GameObject*  gameObject); // Removes the given game object.
        void             Remove      (const size_t&      id);         // Removes the game object with the given ID.
        ObjectTag*       CreateTag   (const std::string& name);       // Creates a new tag with the given name and returns it.
        
        // -- Handling game objects -- //
        void StartAll();
        void UpdateAll(const float& deltaTime, const Core::WindowInputs& inputs, const bool& updateScripts = true) const; // Updates all game objects.
        void DrawAll  (const Render::Renderer* renderer, const Render::Camera* camera, const Render::LightManager* lightManager) const; // Renders all game objects with the given camera and light manager.
        void ClearAll(); // Deletes all game objects.

        // -- Searching for game objects -- //
        GameObject* Find       (const size_t& searchId) const;                          // Tries to find a game object with the given ID.
        GameObject* Find       (const std::string& searchName) const;                   // Tries to find a game object with the given name.
        GameObject* FindWithTag(const std::string& searchName, const std::string& tag); // Tries to find a game object with the given name and tag.

        GameObject* GetRoot() const { return root; } // Returns the root game object.
        std::vector<GameObject*> GetGameObjects() const { if(this) return gameObjects; }
        LightJson::JsonElement* Serialize() const                   override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}

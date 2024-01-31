#pragma once

#include <string>
#include <vector>

#include "ObjectTag.h"
#include "Maths/Transform.h"
#include "Utils/IdGenerator.h"

#include "../Externals/Includes/LightJson/JsonVirtual.h"


namespace Render    { class Renderer; class LightManager; class Light; class Camera; }
namespace Resources { class Cubemap; class Mesh; class Sound;  }
namespace Core      { class Engine; struct WindowInputs; }
namespace Scripts   { struct ObjectScript;   }
namespace Physic    { class RigidbodyVolume; }
namespace Interface { class Widget; class TextWidget; class ButtonWidget; }

namespace Scenes 
{
    struct ObjectTag;
    class Scene;

    // - GameObjectType - //
    enum class GameObjectType
    {
        Empty,
        Model,
        Instanced,
        Camera,
        Cubemap,
        Sound,
        Canvas,
        Light,
    };
    std::string GameObjectTypeToString(const GameObjectType& type);
    GameObjectType GameObjectTypeFromString(const std::string& type);

    // - GameObject - //
    class GameObject : public LightJson::JsonVirtual
    {
    protected:
        bool                                shouldDestroy    = false;
        size_t                              id               = 0;
        GameObjectType                      type             = GameObjectType::Empty;
        std::vector<Scripts::ObjectScript*> scripts          = {};
        std::vector<ObjectTag*>             tags;

        GameObject*              parent   = nullptr;
        std::vector<GameObject*> children = {};

        Scene* scene = nullptr;
        
    public:
        // -- Attributes -- //
        std::string      name;
        Maths::Transform transform;
        Physic::RigidbodyVolume* rigidBody = nullptr;

        // -- Constructor -- //

        // Creates a game object with the given ID, name, parent and type.
        GameObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent = nullptr, const GameObjectType& _type = GameObjectType::Empty);
        ~GameObject() override;
        GameObject(const GameObject&)            = delete;
        GameObject(GameObject&&)                 = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&)      = delete;

        // -- Methods -- //
        void PlayModeStart    () const; // Calls every object script's Start method.
        void PlayModeLateStart() const; // Calls every object script's LateStart method.
        void RecursiveUpdate(const float& deltaTime, const Core::WindowInputs& inputs, const bool& updateScripts = true); // Calls every object script's Update method and updates children.

        void                       AddScript   (Scripts::ObjectScript* script);       // Adds the given script to this game object's scrpits.
        Scripts::ObjectScript*     GetScript   (const size_t& idx) const;             // Returns the script at the given index.
        void                       RemoveScript(const Scripts::ObjectScript* script); // Deletes the given script and removes it from this game object's scripts.
        void                       RemoveScript(const size_t& idx);                   // Deletes the script at the given index and removes it from this game object's scripts.
        
        void AddChild   (GameObject*   child);     // Adds the given game object to this game object's children.
        void RemoveChild(const size_t& childId);   // Removes the child with the given id from this game object's children.
        bool SetParent  (GameObject*   newParent); // Sets this game object's parent to the given game object.

        bool GetDestroyState() const { return shouldDestroy; }
        void SetDestroyState() { shouldDestroy = true; }
        
        std::vector<ObjectTag*> GetTags() const                 { return tags; }                                          // Gets all tags of game object
        void                    AddTag(ObjectTag* tag)          { tags.emplace_back(tag); }                               // Adds the given tag to game object
        void                    RemoveTag(const ObjectTag* tag) { tags.erase(std::find(tags.begin(), tags.end(), tag)); } // Removes the given tag to game object
        void                    ClearTags()                     { tags.clear(); }                                         // Removes all tags to game object
        
        size_t         GetId  () const { return id; }                              // Returns this game object's ID.
        GameObjectType GetType() const { return type; }                            // Returns this game object's type.
        GameObject*    GetParent() const { return parent; }                        // Returns this game object's parent.
        std::vector<GameObject*> GetChildren() const { return children; }          // Returns a vector of all of this game object's children.
        std::vector<Scripts::ObjectScript*> GetScripts() const { return scripts; } // Returns a vector of all this game object's scripts.
        GameObject* FindInChildren(const size_t&      searchId  ) const;           // Searches recursively in children to find and return a game object with the given ID.
        GameObject* FindInChildren(const std::string& searchName) const;           // Searched recursively in children to find and return a game object with the given name.
        
        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - ModelObject (inherits from GameObject) - //
    class ModelObject : public GameObject
    {
    public:
        // -- Attributes -- //
        Resources::Mesh* mesh = nullptr;

        // -- Constructor -- //

        // Creates a model object with the given ID, name, mesh and parent.
        ModelObject(Scene* _scene, const size_t& _id, const std::string& _name, Resources::Mesh* _mesh, GameObject* _parent = nullptr);

        // -- Methods -- //

        // Renders this model object with the given camera and light manager.
        void Draw(const Render::Renderer* renderer, const Render::Camera *camera, const Render::LightManager* lightManager) const;

        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;

    };

    // - InstancedObject (inherits from GameObject) - //
    class InstancedObject : public GameObject
    {
    public:
        // -- Attributes -- //
        Resources::Mesh*              mesh          = nullptr;
        size_t                        instanceCount = 1;
        std::vector<Maths::Transform> transforms    = {};

        // -- Constructor -- //

        // Creates an instanced object with the given ID, name, mesh and parent.
        InstancedObject(Scene* _scene, const size_t& _id, const std::string& _name, Resources::Mesh* _mesh, const size_t& _instanceCount, GameObject* _parent = nullptr);

        // -- Methods -- //
        
        // Renders this instanced object with the given camera and light manager.
        void Draw(const Render::Renderer* renderer, const Render::Camera* camera, const Render::LightManager* lightManager);

        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - CameraObject (inherits from GameObject) - //
    class CameraObject : public GameObject
    {
    public:
        // -- Attributes -- //
        Render::Camera* camera = nullptr;

        // -- Constructor -- //

        // Creates a camera object with the given ID, name, camera and parent.
        CameraObject(Scene* _scene, const size_t& _id, const std::string& _name, Render::Camera* _camera, GameObject* _parent = nullptr);

        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - LightObject (inherits from GameObject) - //
    class LightObject : public GameObject
    {
    public:
        // -- Attributes -- //
        Render::Light* light = nullptr;

        // -- Constructor -- //

        // Creates a light object with the given ID, name, light and parent.
        LightObject(Scene* _scene, const size_t& _id, const std::string& _name, Render::Light* _light, GameObject* _parent = nullptr);

        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - Cubemap (inherits from GameObject) - //
    class CubemapObject : public GameObject
    {
    public:
        // -- Attributes -- //
        Resources::Mesh* mesh       = nullptr;
        Resources::Cubemap* cubemap = nullptr;

        // -- Constructor -- //

        // Creates a cubemap object with the given ID, name, mesh, cubemap and parent.
        CubemapObject(Scene* _scene, const size_t& _id, const std::string& _name, Resources::Cubemap* _cubemap, GameObject* _parent = nullptr);

        // -- Methods -- //

        // Renders this cubemap object with the given camera and light manager.
        void Draw(const Render::Renderer* renderer, const Render::Camera *camera, const Render::LightManager* lightManager) const;

        LightJson::JsonElement* Serialize() const                   override; 
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - Cubemap (inherits from GameObject) - //
    class CanvasObject : public GameObject
    {
    private:
        // -- Attributes -- //
        Utils::IdGenerator              idGenerator;
        std::vector<Interface::Widget*> widgets    = {};
        Interface::Widget*              rootWidget = nullptr;
        friend Interface::Widget;
        
    public:
        // -- Constructors & Destructor -- //
        CanvasObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent = nullptr);
        ~CanvasObject() override;
        CanvasObject(const CanvasObject&)            = delete;
        CanvasObject(CanvasObject&&)                 = delete;
        CanvasObject& operator=(const CanvasObject&) = delete;
        CanvasObject& operator=(CanvasObject&&)      = delete;

        // -- Methods -- //
        Interface::Widget*       AddEmptyWidget (const std::string& widgetName, Interface::Widget* widgetParent = nullptr); // Creates a new empty widget with the given name.
        Interface::TextWidget*   AddTextWidget  (const std::string& widgetName, Interface::Widget* widgetParent = nullptr); // Creates a new text widget with the given name.
        Interface::ButtonWidget* AddButtonWidget(const std::string& widgetName, Interface::Widget* widgetParent = nullptr); // Creates a new button widget with the given name.
        void                     RemoveWidget   (const Interface::Widget* widget);   // Removes the given widget.
        void                     RemoveWidget   (const size_t&            widgetId); // Removes the widget with the given ID.
        
        Interface::Widget* FindWidget(const size_t& widgetId);
        Interface::Widget* FindWidget(const std::string& widgetName);
        Interface::Widget* GetRoot() const { return rootWidget; }

        void Update(const Core::WindowInputs& inputs) const; // Updates the widgets in this canvas object.
        void Draw  (const Render::Renderer* renderer) const; // Renders the widgets in this canvas object.

        LightJson::JsonElement* Serialize() const                   override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };

    // - SoundObject (inherits from GameObject) - //
    class SoundObject : public GameObject
    {
    private:
        // -- Attributes -- //
        std::vector<Resources::Sound*> sounds = {};
        Resources::Sound*              currentSound;

    public:
        // -- Constructors & Destructor -- //
        SoundObject(Scene* _scene, const size_t& _id, const std::string& _name, GameObject* _parent = nullptr);
        ~SoundObject() override = default;
        SoundObject(const SoundObject&) = delete;
        SoundObject(SoundObject&&) = delete;
        SoundObject& operator=(const SoundObject&) = delete;
        SoundObject& operator=(SoundObject&&) = delete;

        // -- Methods -- //
        void AddSound (const std::string& filename);

        void PlayCurrentSound();

        // -- Setters -- //
        void SetCurrentSound(Resources::Sound* sound);

        // -- Getters -- //
        Resources::Sound*               GetCurrentSound() const { return currentSound; }
        std::vector<Resources::Sound*>& GetSounds()             { return sounds;       }

        LightJson::JsonElement* Serialize() const                   override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}

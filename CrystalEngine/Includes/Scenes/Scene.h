#pragma once

#include <string>
#include <vector>

#include "Resources/AssimpParser.h"

#include "../Externals/Includes/LightJson/JsonVirtual.h"

namespace Render    { class Renderer; class CameraManager; class LightManager; }
namespace Core      { class EventDispatcher; struct WindowInputs; class SoundManager; }
namespace Resources { class ResourceManager; class RenderTexture; }
namespace Physic    { class PhysicSystem; class Particle; }
namespace Scenes
{
    class SceneGraph;
    
    enum class SceneState
    {
        Unloaded,
        Loaded,
        Playing,
        Paused,
    };

    // - Scene: handles game object update and rendering - //
    class Scene
    {
    private:
        std::string              filename;
        SceneState               state    = SceneState::Unloaded;
        LightJson::JsonObject*   jsonData = nullptr;
        std::vector<std::string> filesUsed {};

        Resources::RenderTexture* renderTexture   = nullptr;
        Render::CameraManager*    cameraManager   = nullptr;
        Render::LightManager*     lightManager    = nullptr;
        SceneGraph*               sceneGraph      = nullptr;
        Physic::PhysicSystem*     physicsSystem   = nullptr;
        
        Core::EventDispatcher*      eventDispatcher = nullptr;
        Resources::ResourceManager* resourceManager = nullptr;
        Core::SoundManager*         soundManager    = nullptr;

    public:
        // -- Constructor -- //
        Scene(std::string _filename); // Creates a scene with the given name (has to be a filename).
        ~Scene();
        Scene(const Scene&)            = delete;
        Scene(Scene&&)                 = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&)      = delete;
        
        // -- Methods -- //
        void Save();                         // Save this scene to the file corresponding to its name.
        void Load(const bool& async = true); // Loads this scene using its name.
        void Unload();                       // Unloads this scene from memory.
        void Reset() const;                  // Resets all game objects to their default transform.
        void Start();                        // Starts play mode and calls every game object's Start method.
        void Pause();                        // Pauses play mode.
        void Resume();                       // Resumes play mode after it has been paused.
        void Stop();                         // Stops play mode and resets the scene.
        void Update(const float& deltaTime, const Core::WindowInputs& inputs) const;           // Updates every game object.
        void Render(const Render::Renderer* renderer, const bool& useEngineCam = false) const; // Renders every game object.

        void AddFile(const std::string& fileName);
        void RemoveFile(const std::string& fileName);
        // -- Getters -- //
        bool                      IsLoaded        () const { return state != SceneState::Unloaded; } // Returns true if the scene is loaded.
        bool                      IsPlaying       () const { return state == SceneState::Playing;  } // Returns true if the scene is playing.
        bool                      IsPaused        () const { return state == SceneState::Paused;   } // Returns true if the scene is paused.
        std::string               GetName         () const { return filename;      } // Returns this scene's name.
        Physic::PhysicSystem*     GetPhysicsSystem() const { return physicsSystem; } // retrun this scene physic Systeme.
        SceneState                GetState        () const { return state;         } // Returns this scene's state.
        Resources::RenderTexture* GetRenderTexture() const { return renderTexture; } // Returns this scene's render texture.
        Render::CameraManager*    GetCameraManager() const { return cameraManager; } // Returns this scene's camera manager.
        Render::LightManager*     GetLightManager () const { return lightManager;  } // Returns this scene's light manager.
        SceneGraph*               GetSceneGraph   () const { return sceneGraph;    } // Returns this scene's scene graph.

    };
}

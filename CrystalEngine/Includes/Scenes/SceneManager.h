#pragma once
#include <unordered_map>
#include <string>

namespace Core   { class Engine; struct WindowInputs; }
namespace Render { class Renderer;      }
namespace Scenes
{
    class Scene;

    // - SceneManager - //
    class SceneManager
    {
    private:
        std::unordered_map<std::string, Scene*> scenes;
        Scene* activeScene = nullptr;
        
        Core::Engine* engine = nullptr;

    public:
        SceneManager();
        ~SceneManager();
        SceneManager(const SceneManager&)            = delete;
        SceneManager(SceneManager&&)                 = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&)      = delete;

        // -- Methods -- //
        bool   Exists(const std::string& name) const; // Returns true if a scene with the given name has been created.
        bool   Loaded(const std::string& name) const; // Returns true if a scene with the given name is loaded.
        Scene* Create(const std::string& name);       // Creates a new scene with the given name and returns it.
        void   Delete(const std::string& name);       // Deletes the scene with the given name if it exists.

        Scene* CreateDefault(const std::string& name);
        
        Scene* Get(const std::string& name) const;            // Returns the scene with the given name if it exists.
        Scene* GetActiveScene() const { return activeScene; } // Returns the currently active scene.
        void   SetActiveScene(const std::string& name, const bool& shouldStart) const; // Changes the currently active scene to the scene with the given name.
        
        void   StartActiveScene () const;                                                                   // Starts the currently active scene.
        void   UpdateActiveScene(const float& deltaTime, const Core::WindowInputs& inputs) const;           // Updates the currently active scene.
        void   RenderActiveScene(const Render::Renderer* renderer, const bool& useEngineCam = false) const; // Renders the currently active scene.
    };
}

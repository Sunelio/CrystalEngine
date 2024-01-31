#pragma once

namespace Debug     { class Logger; }
namespace Render    { class Renderer; class PostProcessor; class Camera; }
namespace Resources { class ResourceManager; }
namespace Scenes    { class SceneManager; }
namespace Core
{
    class  Engine;
    class  EventDispatcher;
    struct WindowParams;
    class  Window;
    class  TimeManager;
    class  ThreadManager;
    class SoundManager;

    // - Engine (singleton) - //
    class Engine
    {
    private:
        inline static Engine* instance = nullptr;

        EventDispatcher*            eventDispatcher = nullptr;
        Debug::Logger*              logger          = nullptr;
        Window*                     window          = nullptr;
        TimeManager*                time            = nullptr;
        ThreadManager*              threadManager   = nullptr;
        Render::Renderer*           renderer        = nullptr;
        Resources::ResourceManager* resourceManager = nullptr;
        Scenes::SceneManager*       sceneManager    = nullptr;
        SoundManager*               soundManager    = nullptr;
        Render::PostProcessor*      postProcessor   = nullptr;
        
    private:
        Engine()  = default;
        
    public:
        ~Engine() = default;
        Engine(const Engine&)            = delete;
        Engine(Engine&&)                 = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&)      = delete;
        
        // -- Static methods -- //
        static Engine* Create();  // Creates a unique instance of the engine.
        static Engine* Get();     // Returns the engine's unique instance.
        static void    Destroy(); // Destroys the engine's instance.

        // -- Methods -- //
        void Init(const WindowParams& windowParams); // Initialized the engine and all its components.
        void ExecuteFrame() const;                   // Update and render frame.
        void EndFrame()     const;                   // Terminate rendering for the current frame.
        void RenderToScreen() const;                 // Render engine to screen for the Game.
        void Quit()         const;                   // Quits the engine and closes the window.
        void Release()      const;                   // Releases all engine components and resources.

        // -- Getters -- //
        EventDispatcher*            GetEventDispatcher() const { return eventDispatcher; } // Returns the event dispatcher.
        Debug::Logger*              GetLogger()          const { return logger;          } // Returns the logger.
        Window*                     GetWindow()          const { return window;          } // Returns the window.
        TimeManager*                GetTimeManager()     const { return time;            } // Returns the time manager.
        ThreadManager*              GetThreadManager()   const { return threadManager;   } // Returns the thread manager.
        Render::Renderer*           GetRenderer()        const { return renderer;        } // Returns the renderer.
        Resources::ResourceManager* GetResourceManager() const { return resourceManager; } // Returns the resource manager.
        Scenes::SceneManager*       GetSceneManager()    const { return sceneManager;    } // Returns the scene manager.
        SoundManager*               GetSoundManager()    const { return soundManager;    } // Returns the sound manager.
        Render::PostProcessor*      GetPostProcessor()   const { return postProcessor;   } // Returns the post processor.
    };
}

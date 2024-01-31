#pragma once
#include <atomic>
#include <filesystem>
#include <unordered_map>

namespace Core
{
    class EventDispatcher;
}

namespace Core { class ThreadManager; }
namespace Resources
{
    class Resource;

    // - ResourceManager: Stores all engine resources - //
    class ResourceManager
    {
    private:
        Core::ThreadManager*   threadManager   = nullptr;
        Core::EventDispatcher* eventDispatcher = nullptr;
        std::unordered_map<std::string, Resource*>              resources;
        std::unordered_map<std::string, std::vector<Resource*>> loadedFiles;
		std::atomic_flag resourcesLock = ATOMIC_FLAG_INIT;

    public:
        ResourceManager();
        ~ResourceManager();
        ResourceManager(const ResourceManager&)            = delete;
        ResourceManager(ResourceManager&&)                 = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&)      = delete;

        // -- Handling files -- //
        void LoadFile  (const std::string& filename, const bool& async = true); // Loads the given file if it is supported and creates new resources according to its contents.
        void UnloadFile(const std::string& filename);                           // Unloads the resources that have been created by the given file.
        std::vector<Resource*> GetFileResources(const std::string& filename);   // Returns a vector containing all resources that have been created by the given file.
        
        // -- Handling resources -- //
        template<typename T> T* Create(const std::string& name, const bool& async = true);      // Creates and loads a new resource with the given name and type.
        template<typename T> T* Get   (const std::string& name, const bool& noWarning = false); // Returns the resource with the given name and type if it exists.
        void                    Delete(const std::string& name); // Deletes the resource with the given name.
        template<typename T> std::vector<T*> GetAll() const; // Returns all resources of the given type (very inefficient).

        // -- Methods -- //
        template<typename T> bool IsSupported()  const; // Returns true if the specified type is supported by the resource manager.
        bool IsFileLoaded(const std::string& filename); // Returns true if a file with the given filename is loaded.
        bool Exists    (const std::string& name);       // Returns true if a resource with the given name has been created.
        bool InPipeline(const std::string& name);       // Returns true if a resource with the given name has been sent to GPU memory.
        void LoadDefaultResources();                    // Loads default engine resources.
    };
}

#include "ResourceManager.inl"

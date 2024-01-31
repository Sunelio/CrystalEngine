#pragma once
#include "ResourceManager.h"
#include "Resource.h"
#include "Core/EventDispatcher.h"
#include "Core/ThreadManager.h"
#include "Resources/Mesh.h"
#include "Resources/Sound.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/ShaderProgram.h"
#include "Debug/Logger.h"

template <typename T> T* Resources::ResourceManager::Create(const std::string& name, const bool& async)
{
    Assert(IsSupported<T>(), "Tried to create resource \"" + name + "\" with unsupported resource type.");
    
    // Get the resource if it exists and create it if not.
    T* resource = Get<T>(name, true);
    resource->AddUser();

    // Add a task to load the resource.
    std::function loadTask = [resource, eventDispatcher = eventDispatcher]
    {
        resource->Load();
        eventDispatcher->FrameEndDispatch(new Core::ResourceLoadedEvent(resource));
    };
    if (async) threadManager->AddTask(loadTask);
    else       loadTask();
    return resource;
}

template <typename T> T* Resources::ResourceManager::Get(const std::string& name, const bool& noWarning)
{
    Assert(IsSupported<T>(), "Tried to get resource \"" + name + "\" with unsupported resource type.");

    // Make sure the resource exists.
    if (!Exists(name)) {
        if (!noWarning) DebugLogWarning("Resource \"" + name + "\" not found.");
        Resource* resource = new T(name);
        while (resourcesLock.test_and_set()) {}
        resources[name] = resource;
        resourcesLock.clear();
        return (T*)resource;
    }
    
    // Get the resource and make sure it has the right type.
    while (resourcesLock.test_and_set()) {}
    Resource* resource = resources[name];
    resourcesLock.clear();
    if (resource->GetType() != T::GetResourceType()) {
        DebugLogError("Resource \"" + name + "\" found with type " + ResourceTypeToString(resource->GetType()) + " when looking for " + ResourceTypeToString(T::GetResourceType()) + ".");
        return nullptr;
    }
    return (T*)resource;
}

template <typename T> std::vector<T*> Resources::ResourceManager::GetAll() const
{
    Assert(IsSupported<T>(), "Tried to get vector of resources with unsupported resource type.");

    std::vector<T*> output;
    for (auto& [key, resource] : resources) {
        if (resource->GetType() == T::GetResourceType()) {
            output.push_back((T*)resource);
        }
    }
    return output;
}

template <typename T> bool Resources::ResourceManager::IsSupported() const
{
    // Make sure the given type is supported.
    return std::is_base_of_v<Resource, T>;
}

#include "Resources/ResourceManager.h"

#include <array>

#include "Core/Engine.h"
#include "Core/ThreadManager.h"
#include "Resources/Resource.h"
#include "Debug/Logger.h"
#include "Resources/AssimpParser.h"
#include "Resources/Font.h"
#include "Resources/Material.h"
#include "Resources/WavefrontParser.h"
#include "Resources/Shader.h"
#include "Resources/ShaderProgram.h"
#include "Resources/SubMesh.h"
#include "Resources/Texture.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/SceneManager.h"
#include "Scripts/DefaultScripts.h"
namespace fs = std::filesystem;
using namespace Resources;
using namespace Core;

ResourceManager::ResourceManager()
{
    const Engine* engine = Engine::Get();
    threadManager   = engine->GetThreadManager();
    eventDispatcher = engine->GetEventDispatcher();

    // Send a resource to the pipeline when it is loaded.
    eventDispatcher->AddListener<ResourceLoadedEvent>([eventDispatcher = eventDispatcher](Event* event)
    {
        const ResourceLoadedEvent* loadedEvent = (ResourceLoadedEvent*)event;
        if (!loadedEvent) return;
        Resource* resource = loadedEvent->resource;
        if (!resource) return;
        resource->SendToPipeline();
        if (resource->WasSentToPipeline()) return;
        eventDispatcher->FutureDispatchFrames(new ResourceLoadedEvent(resource), 1);
    });
}

ResourceManager::~ResourceManager()
{
    while (resourcesLock.test_and_set()) {}
    for (const auto& [name, resource] : resources) 
        delete resource;
    resources.clear();
    resourcesLock.clear();
}

void ResourceManager::LoadDefaultResources()
{
    const std::string shaderPath  = "Resources/Shaders/";
    const std::string meshPath    = "Resources/Meshes/";
    const std::string texturePath = "Resources/Textures/";
    const std::string fontPath    = "Resources/Fonts/";
    const std::string soundPath    = "Resources/Sounds/";

    
    const std::vector defaultFiles = {
        shaderPath  + "Mesh/meshShader.vert",
        shaderPath  + "Screen/screenShader.vert",
        shaderPath  + "Mesh/animShader.vert",
        shaderPath  + "Mesh/skyboxShader.vert",
        shaderPath  + "Mesh/gridShader.vert",
        shaderPath  + "Mesh/meshShader.frag",
        shaderPath  + "Mesh/instancedMeshShader.vert",
        shaderPath  + "Mesh/instancedAnimShader.vert",
        shaderPath  + "Mesh/skyboxShader.frag",
        shaderPath  + "Screen/screenShader.frag",
        shaderPath  + "Screen/edgeDetection.frag",
        shaderPath  + "Screen/textShader.frag",
        shaderPath  + "Mesh/gridShader.frag",

        fontPath    + "Ghastly Panic.ttf",

        meshPath    + "BasicMesh/Cube.obj",
        meshPath    + "BasicMesh/Plane.obj",
        meshPath    + "Gizmo/gizmoTranslation.obj",
        meshPath    + "Gizmo/gizmoRotation.obj",
        meshPath    + "Cubemap/cubemap.obj",
        meshPath    + "Gizmo/gizmoScaling.obj",
        meshPath    + "Environement/tree.obj",
        meshPath    + "Environement/floor/floor.obj",
        meshPath    + "slender/slenderMan.fbx",
        meshPath    + "trees/DeadOak1.fbx",
        meshPath    + "trees/DeadOak2.fbx",
        meshPath    + "trees/DeadOak3.fbx",
        meshPath    + "trees/DeadSpruce1.fbx",
        meshPath    + "Rock/BigRock1.fbx",
        meshPath    + "Rock/Rock_10.fbx",
        meshPath    + "Rock/Rock1.fbx",

        soundPath   + "Dark-Forest.mp3",
        soundPath   + "Chase-1.mp3",
        soundPath   + "Jumpscare.mp3",
        
        texturePath + "defaultTexture.png",
        texturePath + "UI/canvasPanel.jpg",
        texturePath + "UI/Button/DefaultButton.png",
        texturePath + "UI/Button/ButtonHovered.png",
        texturePath + "UI/Button/ButtonPress.png",
    };
    
    const std::vector<std::array<std::string, 3>> shaderPrograms = {
        { "Mesh/gridShader.vert",          "Mesh/gridShader.frag",      "Mesh/gridShader.prog"          },
        { "Mesh/meshShader.vert",          "Mesh/meshShader.frag",      "Mesh/meshShader.prog"          },
        { "Mesh/animShader.vert",          "Mesh/meshShader.frag",      "Mesh/animShader.prog"          },
        { "Mesh/instancedMeshShader.vert", "Mesh/meshShader.frag",      "Mesh/instancedMeshShader.prog" },
        { "Mesh/instancedAnimShader.vert", "Mesh/meshShader.frag",      "Mesh/instancedAnimShader.prog" },
        { "Mesh/skyboxShader.vert",        "Mesh/skyboxShader.frag",    "Mesh/skyboxShader.prog"        },
        { "Screen/screenShader.vert",      "Screen/screenShader.frag",  "Screen/screenShader.prog"      },
        { "Screen/screenShader.vert",      "Screen/edgeDetection.frag", "Screen/edgeDetection.prog"     },
        { "Screen/screenShader.vert",      "Screen/textShader.frag",    "Screen/textShader.prog"        },
    };

    for (const std::string& file : defaultFiles)
        LoadFile(file, false);

    for (const std::array<std::string, 3>& shaders : shaderPrograms)
    {
        VertexShader*   vertexShader   = Get   <VertexShader  >(shaderPath + shaders[0]);
        FragmentShader* fragmentShader = Get   <FragmentShader>(shaderPath + shaders[1]);
        ShaderProgram*  shaderProgram  = Create<ShaderProgram >(shaderPath + shaders[2], false);
        shaderProgram->AttachShaders(2, vertexShader, fragmentShader);
        shaderProgram->Load();
    }

    if (const Mesh* mesh = Get<Mesh>("mesh_cubemap"); !mesh->subMeshes.empty())
        mesh->subMeshes[0]->SetShaderProgram(Get<ShaderProgram>(shaderPath + "Mesh/skyboxShader.prog"));
    
    if (ShaderProgram* shader = Get<ShaderProgram>(shaderPath + "Mesh/skyboxShader.prog"))
        shader->SetScript(new Scripts::SkyboxMeshShaderScript);
    
    if (ShaderProgram* shader = Get<ShaderProgram>(shaderPath + "Screen/screenShader.prog"))
        shader->SetScript(new Scripts::DefaultScreenShaderScript);
    
    if (ShaderProgram* shader = Get<ShaderProgram>(shaderPath + "Screen/edgeDetection.prog"))
        shader->SetScript(new Scripts::DefaultScreenShaderScript);
}


void ResourceManager::LoadFile(const std::string& filename, const bool& async)
{
    if(filename.empty()) return;
    
    std::function loadFileTask = [this, filename = std::string(filename)]
    {
        const fs::path      filepath  = fs::path(filename);
        const std::string   extension = filepath.extension().string().substr(1);
        const std::function addLoadedFile = [this, filename](std::vector<Resource*> fileResources)
        {
            while (resourcesLock.test_and_set()) {}
            loadedFiles[filename] = std::move(fileResources);
            resourcesLock.clear();
        };
        
        if (extension == "png" || extension == "jpg") {
            addLoadedFile(std::vector<Resource*>{ Create<Texture>(filename, false) });
        }
        else if (extension == "vert") {
            addLoadedFile(std::vector<Resource*>{ Create<VertexShader>(filename, false) });
        }
        else if (extension == "frag") {
            addLoadedFile(std::vector<Resource*>{ Create<FragmentShader>(filename, false) });
        }
        else if (extension == "obj") {
            addLoadedFile(WavefrontParser::ParseObj(filename));
        }
        else if (extension == "mtl") {
            addLoadedFile(WavefrontParser::ParseMtl(filename));
        }
        else if (extension == "fbx") {
            addLoadedFile(std::vector{ (Resource*)AssimpParser::ParseModelFile(filename) });
        }
        else if (extension == "ttf") {
            addLoadedFile(std::vector<Resource*>{ Create<Font>(filename, false) });
        }
        else if (extension == "wav" || extension == "mp3" || extension == "ogg") {
            addLoadedFile(std::vector<Resource*>{ Create<Sound>(filename, false) });
        }
    };
    if (async) threadManager->AddTask(loadFileTask);
    else       loadFileTask();
}

void ResourceManager::UnloadFile(const std::string& filename)
{
    if (!IsFileLoaded(filename)) return;
    std::vector<Resource*>& fileResources = loadedFiles[filename];
    for (int i = (int)fileResources.size()-1; i >= 0; i--)
    {
        const Resource* resource = fileResources[i];
        const std::vector<Scenes::GameObject*> gameObjects = Engine::Get()->GetSceneManager()->GetActiveScene()->GetSceneGraph()->GetGameObjects();
        if (resource->GetUserCount() <= 1)
        {
            for (const Scenes::GameObject* gameObject : gameObjects)
            {
                switch (gameObject->GetType()) {
                case Scenes::GameObjectType::Model:
                case Scenes::GameObjectType::Instanced:
                    {
                        Scenes::ModelObject* object = (Scenes::ModelObject*)gameObject;
                        if (object->mesh && object->mesh->GetName() == resource->GetName())
                        {
                            object->SetDestroyState();
                            Engine::Get()->GetSceneManager()->GetActiveScene()->GetSceneGraph()->Remove(object);
                            object->mesh = nullptr;
                            break;
                        }

                        for (SubMesh* subMesh : object->mesh->subMeshes)
                        {
                            Material* material = subMesh->GetMaterial();
                            if (material && material->GetName() == resource->GetName())
                            {
                                if (material->GetName() == resource->GetName())
                                {
                                    subMesh->SetMaterial(nullptr);
                                    continue;
                                }

                                for (Texture** texture : material->GetTextures())
                                {
                                    if (*texture && (*texture)->GetName() == resource->GetName())
                                    {
                                        *texture = nullptr;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case Scenes::GameObjectType::Sound:
                    {
                        Scenes::SoundObject* object = (Scenes::SoundObject*)gameObject;
                        for (Sound*& sound : object->GetSounds())
                        {
                            if (sound->GetName() == resource->GetName())
                            {
                                sound = nullptr;
                            }
                        }
                        break;
                    }
                default: break;
                }
            }
            fileResources.erase(fileResources.begin()+i);
        }
        
        Delete(resource->GetName());
    }
    if (fileResources.empty())
        loadedFiles.erase(filename);
}

std::vector<Resource*> ResourceManager::GetFileResources(const std::string& filename)
{
    if (!IsFileLoaded(filename)) return std::vector<Resource*>();
    return loadedFiles[filename];
}

void ResourceManager::Delete(const std::string& name)
{
    if (!Exists(name)) {
        DebugLogWarning("Tried to delete resource \"" + name + "\" that doesn't exist!");
        return;
    }
    if (!InPipeline(name)) {
        DebugLogWarning("Unable to delete resource \"" + name + "\" while it is loading.");
        return;
    }
    while (resourcesLock.test_and_set()) {}
    Resource* resource = resources[name];
    resource->DelUser();
    if (!resource->IsUsed())
    {
        delete resource;
        resources.erase(name);
    }
    resourcesLock.clear();
}

bool ResourceManager::IsFileLoaded(const std::string& filename)
{
    while (resourcesLock.test_and_set()) {}
    const bool exists = loadedFiles.count(filename) > 0;
    resourcesLock.clear();
    return exists;
}

bool ResourceManager::Exists(const std::string& name)
{
    while (resourcesLock.test_and_set()) {}
    const bool exists = resources.count(name) > 0;
    resourcesLock.clear();
    return exists;
}

bool ResourceManager::InPipeline(const std::string& name)
{
    while (resourcesLock.test_and_set()) {}
    bool inPipeline = false;
    if (resources.count(name) > 0)
        inPipeline = resources.at(name)->WasSentToPipeline();
    resourcesLock.clear();
    return inPipeline;
}

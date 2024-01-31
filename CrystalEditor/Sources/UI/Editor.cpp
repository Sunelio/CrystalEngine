#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#include "UI/Editor.h"
#include "Debug/Logger.h"

#include "Core/Engine.h"
#include "Core/TimeManager.h"
#include "Core/Window.h"

#include "Render/Renderer.h"
#include "Render/PostProcessor.h"
#include "Render/CameraManager.h"
#include "Render/Light.h"
#include "Render/Camera.h"
#include "Render/LightManager.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"

#include "Interface/Widget.h"

#include "Resources/Mesh.h"
#include "Resources/Resource.h"
#include "Resources/Material.h"
#include "Resources/ResourceManager.h"
#include "Resources/RenderTexture.h"
#include "Resources/SubMesh.h"
#include "Resources/Texture.h"
#include "Resources/ShaderProgram.h"
#include "Resources/RenderTexture.h"
#include "Resources/Font.h"
#include "Resources/Cubemap.h"
#include "Resources/Sound.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

#include "Scripts/DefaultScripts.h"

#include "Utils/WindowsConverter.h"

#include <sstream>
#include <imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <filesystem>
#include <utility>
#include <imgui_internal.h>
#include <optional>
#include <format>

#define NOMINMAX
#include <fstream>
#include <windows.h>

#include "Core/SoundManager.h"
#include "Scripts/LuaScript.h"
#include "Scripts/PlayerMovement.h"
#include "Scripts/PythonScript.h"

#undef near
#undef max
#undef far

namespace fs = std::filesystem;
using namespace Maths;
using namespace Debug;
using namespace Core;
using namespace Resources;
using namespace Render;
using namespace Scenes;
using namespace Interface;
using namespace Anim;
using namespace UI;
using namespace Scripts;
#pragma warning(disable: 4312) // Used to silence to ImTextureID cast warning.


// ---- Constant file-scope definitions ---- //

constexpr const char* iniFilename = "Resources/imgui.ini";

const std::string iconFolder = "Resources/Textures/Logos/";
const std::string scriptingIconPaths[3] = {
    iconFolder + "Scripting/cpp.png",
    iconFolder + "Scripting/python.png",
    iconFolder + "Scripting/lua.png",
};
const std::string fileIconPaths[11] = {
    iconFolder + "Files/code.png",
    iconFolder + "Files/image.png",
    iconFolder + "Files/material.png",
    iconFolder + "Files/mesh.png",
    iconFolder + "Files/scene.png",
    iconFolder + "Files/settings.png",
    iconFolder + "Files/shader.png",
    iconFolder + "Files/font.png",
    iconFolder + "Files/audio.png",
    iconFolder + "Files/document.png",
    iconFolder + "Files/folder.png",
};
const std::string miscIconPaths[21] = {
    iconFolder + "Miscs/add.png",
    iconFolder + "Miscs/x.png",
    iconFolder + "Miscs/y.png",
    iconFolder + "Miscs/z.png",
    iconFolder + "Miscs/transform.png",
    iconFolder + "Miscs/colors.png",
    iconFolder + "Miscs/properties.png",
    iconFolder + "Miscs/mesh.png",
    iconFolder + "Miscs/material.png",
    iconFolder + "Miscs/skeleton.png",
    iconFolder + "Miscs/animator.png",
    iconFolder + "Miscs/remove.png",
    iconFolder + "Miscs/edit.png",
    iconFolder + "Miscs/sound.png",
    iconFolder + "Miscs/delete.png",
    iconFolder + "Miscs/load.png",
    iconFolder + "Miscs/unload.png",
    iconFolder + "Miscs/upload.png",
    iconFolder + "Miscs/transforms.png",
    iconFolder + "Miscs/settings.png",
    iconFolder + "Miscs/script.png",
};
const std::string objectIconPaths[10] = {
    iconFolder + "Objects/empty.png",
    iconFolder + "Objects/model.png",
    iconFolder + "Objects/instanced.png",
    iconFolder + "Objects/camera.png",
    iconFolder + "Objects/cubemap.png",
    iconFolder + "Objects/sound.png",
    iconFolder + "Objects/canvas.png",
    iconFolder + "Objects/directionalLight.png",
    iconFolder + "Objects/pointLight.png",
    iconFolder + "Objects/spotLight.png",
};
const std::string widgetIconPaths[3] = {
    iconFolder + "Widgets/empty.png",
    iconFolder + "Widgets/button.png",
    iconFolder + "Widgets/text.png",
};
const std::string graphIconPaths[11] = {
    iconFolder + "Graph/empty.png",
    iconFolder + "Graph/model.png",
    iconFolder + "Graph/instanced.png",
    iconFolder + "Graph/camera.png",
    iconFolder + "Graph/cubemap.png",
    iconFolder + "Graph/sound.png",
    iconFolder + "Graph/canvas.png",
    iconFolder + "Graph/light.png",
    iconFolder + "Graph/root.png",
    iconFolder + "Graph/text.png",
    iconFolder + "Graph/button.png",
};

const std::vector<std::vector<std::string>> fileExtensions = {
    { ".lua", ".py", ".cpp" },
    { ".png", ".jpg" },
    { ".mtl" },
    { ".obj", ".fbx" },
    { ".json" },
    { ".ini" },
    { ".frag", ".vert", ".comp" },
    { ".ttf" },
    { ".wav", ".mp3" },
};


// ---- Editor code ---- //

Editor::Editor(Engine* _engine)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Set input/output parameters.
    imGuiIO = &ImGui::GetIO();
    imGuiIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imGuiIO->IniFilename  = NULL;
    ImGui::LoadIniSettingsFromDisk(iniFilename);
    imGuiIO->FontDefault = imGuiIO->Fonts->AddFontFromFileTTF("Resources/Fonts/JetBrainsMono-Regular.ttf", 17);

    // Set ImGui style.
    ImGui::StyleColorsDark();
    imGuiStyle = &ImGui::GetStyle();
    imGuiStyle->WindowPadding       = { 6, 6 };
    imGuiStyle->FramePadding        = { 6, 3 };
    imGuiStyle->ItemSpacing         = { 6, 4 };
    imGuiStyle->IndentSpacing       = 15;
    imGuiStyle->ScrollbarSize       = 12;
    imGuiStyle->WindowRounding      = 5;
    imGuiStyle->ChildRounding       = 5;
    imGuiStyle->FrameRounding       = 5;
    imGuiStyle->PopupRounding       = 5;
    imGuiStyle->ScrollbarRounding   = 5;
    imGuiStyle->GrabRounding        = 5;
    imGuiStyle->TabRounding         = 5;
    imGuiStyle->ColorButtonPosition = ImGuiDir_Left;
    imGuiStyle->Colors[ImGuiCol_Border            ] = { 0.502f, 0.431f, 0.475f, 0.502f };
    imGuiStyle->Colors[ImGuiCol_BorderShadow      ] = { 0.502f, 0.431f, 0.475f, 0.502f };
    imGuiStyle->Colors[ImGuiCol_FrameBg           ] = { 0.478f, 0.161f, 0.353f, 0.541f };
    imGuiStyle->Colors[ImGuiCol_FrameBgHovered    ] = { 0.980f, 0.263f, 0.702f, 0.400f };
    imGuiStyle->Colors[ImGuiCol_FrameBgActive     ] = { 0.980f, 0.263f, 0.702f, 0.671f };
    imGuiStyle->Colors[ImGuiCol_TitleBgActive     ] = { 0.000f, 0.000f, 0.000f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_CheckMark         ] = { 0.980f, 0.263f, 0.702f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_SliderGrab        ] = { 0.878f, 0.239f, 0.631f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_SliderGrabActive  ] = { 0.980f, 0.263f, 0.702f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_Button            ] = { 0.980f, 0.263f, 0.702f, 0.400f };
    imGuiStyle->Colors[ImGuiCol_ButtonHovered     ] = { 0.980f, 0.263f, 0.702f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_ButtonActive      ] = { 0.980f, 0.063f, 0.624f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_Header            ] = { 0.980f, 0.263f, 0.702f, 0.310f };
    imGuiStyle->Colors[ImGuiCol_HeaderHovered     ] = { 0.980f, 0.263f, 0.702f, 0.800f };
    imGuiStyle->Colors[ImGuiCol_HeaderActive      ] = { 0.980f, 0.263f, 0.702f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_Separator         ] = { 0.424f, 0.137f, 0.314f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_SeparatorHovered  ] = { 0.749f, 0.098f, 0.498f, 0.780f };
    imGuiStyle->Colors[ImGuiCol_SeparatorActive   ] = { 0.702f, 0.098f, 0.498f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_ResizeGrip        ] = { 0.980f, 0.263f, 0.702f, 0.200f };
    imGuiStyle->Colors[ImGuiCol_ResizeGripHovered ] = { 0.980f, 0.263f, 0.702f, 0.671f };
    imGuiStyle->Colors[ImGuiCol_ResizeGripActive  ] = { 0.980f, 0.263f, 0.702f, 0.949f };
    imGuiStyle->Colors[ImGuiCol_Tab               ] = { 0.580f, 0.180f, 0.424f, 0.863f };
    imGuiStyle->Colors[ImGuiCol_TabHovered        ] = { 0.980f, 0.263f, 0.702f, 0.800f };
    imGuiStyle->Colors[ImGuiCol_TabActive         ] = { 0.678f, 0.200f, 0.494f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_TabUnfocused      ] = { 0.149f, 0.067f, 0.118f, 0.973f };
    imGuiStyle->Colors[ImGuiCol_TabUnfocusedActive] = { 0.424f, 0.137f, 0.314f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_DockingPreview    ] = { 0.980f, 0.263f, 0.702f, 0.702f };
    imGuiStyle->Colors[ImGuiCol_TableHeaderBg     ] = { 0.200f, 0.188f, 0.196f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_TableBorderStrong ] = { 0.349f, 0.310f, 0.333f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_TableBorderLight  ] = { 0.251f, 0.231f, 0.243f, 1.000f };
    imGuiStyle->Colors[ImGuiCol_TextSelectedBg    ] = { 0.980f, 0.263f, 0.702f, 0.349f };
    imGuiStyle->Colors[ImGuiCol_NavHighlight      ] = { 0.980f, 0.263f, 0.702f, 1.000f };
    
    // Get engine classes.
    engine          = _engine;
    window          = engine->GetWindow();
    resourceManager = engine->GetResourceManager();
    sceneManager    = engine->GetSceneManager();
    renderer        = engine->GetRenderer();
    postProcessor   = engine->GetPostProcessor();
    
    // Initialize ImGui for OpenGL.
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindowContext(), true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // Enable rendering of game objects to the stencil buffer.
    renderer->RenderObjectsToStencil(true);

    // Create the selected object render texture.
    engineRenderTexture = new RenderTexture("EngineRenderTexture");
    engineRenderTexture->clearColor = { 0, 0, 0, 1 };
    engineRenderTexture->SetSize(window->GetWidth(), window->GetHeight());
    engineRenderTexture->Load(); engineRenderTexture->SendToPipeline();

    // Create the selected object render texture.
    viewportRenderTexture = new RenderTexture("ViewportRenderTexture");
    viewportRenderTexture->clearColor = { 0, 0, 0, 1 };
    viewportRenderTexture->SetSize(window->GetWidth(), window->GetHeight());
    viewportRenderTexture->Load(); viewportRenderTexture->SendToPipeline();

    // Create the selected object render texture.
    objectRenderTexture = new RenderTexture("ObjectRenderTexture");
    objectRenderTexture->clearColor = { 0, 0, 0, 0 };
    objectRenderTexture->SetSize(window->GetWidth(), window->GetHeight());
    objectRenderTexture->Load(); objectRenderTexture->SendToPipeline();

    // Create canvas panel render texture.
    canvasRenderTexture = new RenderTexture("CanvasPanelRendertexture");
    canvasRenderTexture->SetSize(window->GetWidth(), window->GetHeight());
    canvasRenderTexture->Load();
    canvasRenderTexture->SendToPipeline();

    // Add listeners to engine events.
    EventDispatcher* eventDispatcher = engine->GetEventDispatcher();
    eventDispatcher->AddListener<ScreenResizeEvent>([this](Event* event)
    {
        const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
        if (!resizeEvent) return;
        engineRenderTexture  ->SetSize(resizeEvent->width, resizeEvent->height);
        viewportRenderTexture->SetSize(resizeEvent->width, resizeEvent->height);
        objectRenderTexture  ->SetSize(resizeEvent->width, resizeEvent->height);
        canvasRenderTexture  ->SetSize(resizeEvent->width, resizeEvent->height);
    });
    eventDispatcher->AddListener<ReloadSceneEvent>([this](Event* event)
    {
        selectedGameObject = nullptr;
        selectedCanvas     = nullptr;
        selectedWidget     = nullptr;
    });
    eventDispatcher->AddListener<SetActiveSceneEvent>([this](Event* event)
    {
        selectedGameObject = nullptr;
        selectedCanvas     = nullptr;
        selectedWidget     = nullptr;
    });
    eventDispatcher->AddListener<RemoveGameObjectEvent>([this](Event* event)
    {
        if (selectedGameObject == (GameObject*)selectedCanvas)
        {
            selectedCanvas = nullptr;
            selectedWidget = nullptr;
        }
        selectedGameObject = nullptr;
    });
    eventDispatcher->AddListener<RemoveWidgetEvent>([this](Event* event)
    {
        selectedWidget = nullptr;
    });

    // Load editor icon textures.
    const std::function loadIcon = [this](const std::string& iconPath)
    {
        Texture* texture = new Texture(iconPath);
        texture->Load();
        texture->SendToPipeline();
        return texture;
    };

    for (const std::string& iconPath : scriptingIconPaths) scriptingIcons[iconPath] = loadIcon(iconPath);
    for (const std::string& iconPath : fileIconPaths     ) fileIcons     [iconPath] = loadIcon(iconPath);
    for (const std::string& iconPath : miscIconPaths     ) miscIcons     [iconPath] = loadIcon(iconPath);
    for (const std::string& iconPath : objectIconPaths   ) objectIcons   [iconPath] = loadIcon(iconPath);
    for (const std::string& iconPath : widgetIconPaths   ) widgetIcons   [iconPath] = loadIcon(iconPath);
    for (const std::string& iconPath : graphIconPaths    ) graphIcons    [iconPath] = loadIcon(iconPath);
}

Editor::~Editor()
{
    for (auto& [key, value] : scriptingIcons) delete value;
    for (auto& [key, value] : fileIcons     ) delete value;
    for (auto& [key, value] : miscIcons     ) delete value;
    for (auto& [key, value] : objectIcons   ) delete value;
    for (auto& [key, value] : widgetIcons   ) delete value;
    for (auto& [key, value] : graphIcons    )  delete value;
    
    ImGui::SaveIniSettingsToDisk(iniFilename);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

void Editor::LockInputsIfUsed() const
{
    window->LockInputs(imGuiIO->WantCaptureMouse || imGuiIO->WantCaptureKeyboard);
}

void Editor::ExecuteFrame()
{
    // Render scene from engine camera.
    postProcessor->BeginRender(engineRenderTexture);
    sceneManager ->RenderActiveScene(renderer, true);
    postProcessor->EndRender(engineRenderTexture);

    // Render ImGui.
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize({ (float)window->GetWidth(), (float)window->GetHeight() });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 22 });
    ImGui::Begin("CrystalEngine", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleVar();
    {
        ImGui::DockSpace(ImGui::GetID("CrystalEngine"));
        ShowMenuBar();
        FileExplorerWindow();
        FileInspectorWindow();
        LogsWindow();
        if (selectedCanvas)
        {
            CanvasPanelWindow();
            WidgetInspectorWindow();
            CanvasGraphWindow();
        }
        else
        {
            PlayWindow();
            SceneGraphWindow();
            ObjectInspectorWindow();
        }
        SceneViewWindow();
        ViewportWindow();
    }
    ImGui::End();

    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        if (Scene* scene = sceneManager->GetActiveScene())
            scene->Save();
    }
}

void Editor::EndFrame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::ShowMenuBar()
{
    bool newSceneButtonPressed    = false;
    bool renameButtonPressed      = false;
    static bool statsWindow       = false;
    static bool settingsWindow    = false;
    static bool postProcessWindow = false;
    
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene"))
        {
            sceneManager->GetActiveScene()->Save();
            
            const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 1.5f));
            newSceneButtonPressed = true;
        }
        if (ImGui::MenuItem("Open Scene"))
        {
            sceneManager->GetActiveScene()->Save();
            try
            {
                const std::filesystem::path scenePath = Utils::WindowsUtils::OpenExplorer();
                const std::string sceneName = std::filesystem::relative(scenePath).string();
                if (sceneManager->Exists(sceneName)) {
                    sceneManager->SetActiveScene(sceneName, false);
                }
                else {
                    sceneManager->Create(sceneName);
                    sceneManager->SetActiveScene(sceneName, false);
                }
            }
            catch (const std::exception&) {
                DebugLogError("Failed to load scene");
            }
        }
        if (ImGui::MenuItem("Save Scene"))
        {
            sceneManager->GetActiveScene()->Save();
            DebugLogInfo("Saved active scene.");
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !((!selectedCanvas && selectedGameObject) || selectedWidget));
        if (ImGui::MenuItem("Rename"))
        {
            ImGui::CloseCurrentPopup();
            renameButtonPressed = true;
        }
        if (ImGui::MenuItem("Remove"))
        {
            sceneManager->GetActiveScene()->GetSceneGraph()->Remove(selectedGameObject);
        }
        ImGui::PopItemFlag();
        ImGui::Separator();

        Scene* scene = sceneManager->GetActiveScene();
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !scene);
        if (ImGui::MenuItem("Play"))
        {
            if (scene->IsPaused())
                scene->Resume();
            else if (!scene->IsPlaying())
                scene->Start();
        }
        if (ImGui::MenuItem("Pause"))
        {
            scene->Pause();
        }
        if (ImGui::MenuItem("Stop"))
        {
            if (scene->IsPlaying() || scene->IsPaused())
                scene->Stop();
        }
        ImGui::PopItemFlag();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::MenuItem("Statistics"))
        {
            ImGui::CloseCurrentPopup();
            statsWindow = !statsWindow;
        }
        if (ImGui::MenuItem("Settings"))
        {
            ImGui::CloseCurrentPopup();
            settingsWindow = !settingsWindow;
        }
        if (ImGui::MenuItem("Post Process"))
        {
            ImGui::CloseCurrentPopup();
            postProcessWindow = !postProcessWindow;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("Documentation"))
        {
            std::system("start https://gitlabstudents.isartintra.com/projets/2022_projet_moteur_gp_2026_projet_moteur-crystal_engine/-/blob/develop/README.md");
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    
    if (!selectedCanvas)
    {
        if (selectedGameObject)
        {
            if (renameButtonPressed)
                ImGui::OpenPopup("GameObjectRenamePopup");
            
            ShowRenamePopup("GameObjectRenamePopup", selectedGameObject->name.c_str(), [selectedGameObject=selectedGameObject](std::string newName) {
                selectedGameObject->name = std::move(newName);
            });
        }
    }
    else
    {
        if (selectedWidget)
        {
            if (renameButtonPressed)
                ImGui::OpenPopup("WidgetRenamePopup");
            
            ShowRenamePopup("WidgetRenamePopup", selectedWidget->name.c_str(), [selectedWidget=selectedWidget](std::string newName) {
                selectedWidget->name = std::move(newName);
            });
        }
    }

    if (newSceneButtonPressed)
        ImGui::OpenPopup("Create scene");
    ShowCreateScenePopup();
    
    if (statsWindow)       ShowStatsWindow();
    if (settingsWindow)    ShowSettingsWindow();
    if (postProcessWindow) ShowPostProcessWindow();
}

void Editor::ShowRenamePopup(const char* strId, const char* originalName, const std::function<void(std::string)>& renameCallback) const
{
    if (ImGui::BeginPopup(strId))
    {
        // Popup title.
        ImGui::Text("Rename %s", originalName);

        // Name input (focus it on the frame when the popup opens).
        static std::string newName;
        ImGui::SetNextItemWidth(135);
        if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);
        ImGui::InputText("##NameInput", &newName);

        // Press enter or confirm button to rename the node.
        if ((ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter) || ImGui::Button("Confirm", {65, 25})) && !newName.empty())
        {
            renameCallback(newName);
            newName = "";
            ImGui::CloseCurrentPopup();
        }

        // Cancel button to close the popup.
        ImGui::SameLine();
        if (ImGui::Button("Cancel", {65, 25}))
        {
            newName = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Editor::ShowDeletePopup(const char* strId, const char* name, const std::function<void()>& deleteCallback) const
{
    if (ImGui::BeginPopup(strId))
    {
        // Popup title.
        ImGui::Text("Delete %s forever?", name);

        // Press enter or confirm button to rename the node.
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter) || ImGui::Button("Confirm", {65, 25}))
        {
            deleteCallback();
            ImGui::CloseCurrentPopup();
        }

        // Cancel button to close the popup.
        ImGui::SameLine();
        if (ImGui::Button("Cancel", {65, 25}))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Editor::ShowFilePopup(const char* strId, const fs::path& file, bool& renameCallback, bool& deleteCallback)
{
    if (ImGui::BeginPopup(strId, ImGuiWindowFlags_NoMove))
    {
        if (ImGui::BeginTable("FileInfoTable", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ShowTextureUi(miscIcons[miscIconPaths[12]], 25);
            ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

            if (ImGui::MenuItem("Rename"))
            {
                renameCallback = true;
                ImGui::CloseCurrentPopup();
            }
                
            if (file.has_extension())
            {
                    
                std::string filePath  = file.string(); std::replace(filePath.begin(), filePath.end(), '\\', '/');
                const bool resourceLoaded = resourceManager->IsFileLoaded(filePath);

                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[resourceLoaded ? 16 : 15]], 25);
                ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
                if (ImGui::MenuItem(resourceLoaded ? "Unload" : "Load"))
                {
                    if (resourceLoaded) {
                        resourceManager->UnloadFile(filePath);
                        sceneManager->GetActiveScene()->RemoveFile(filePath);
                    }
                    else {
                        resourceManager->LoadFile(filePath);
                        sceneManager->GetActiveScene()->AddFile(filePath);
                    }
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ShowTextureUi(miscIcons[miscIconPaths[14]], 25);
            ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
            if (ImGui::MenuItem("Delete"))
            {
                deleteCallback = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndTable();
        }
        ImGui::EndPopup();
    }
}

void Editor::ShowTransformUi(Transform& transform, const std::string& id) const
{
    const std::string headerText = "Transform##3D" + id;

    const auto texture = miscIcons.at(miscIconPaths[4]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(headerText.c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        if (ImGui::BeginTable("PositionTable", 7, ImGuiTableFlags_SizingFixedFit))
        {
            // Inputs for position.
            Vector3 objectPos = transform.GetPosition();
        
            ImGui::TableNextColumn();
            ImGui::Text("Position");
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            if (ImGui::DragFloat("##PositionX", &objectPos.x, 0.1f)) {
                transform.SetPosition(objectPos);
            }
        
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            
            if (ImGui::DragFloat("##PositionY", &objectPos.y, 0.1f)) {
                transform.SetPosition(objectPos);
            }
        
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            if (ImGui::DragFloat("##PositionZ", &objectPos.z, 0.1f)) {
                transform.SetPosition(objectPos);
            }
        
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("RotationTable", 7, ImGuiTableFlags_SizingFixedFit))
        {
            // Inputs for rotation.
            Vector3 objectRot = transform.GetEulerRot();
            objectRot.x = radToDeg(objectRot.x); objectRot.y = radToDeg(objectRot.y); objectRot.z = radToDeg(objectRot.z);
        
            ImGui::TableNextColumn();
            ImGui::Text("Rotation");
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            
            if (ImGui::DragFloat("##RotationX", &objectRot.x, 0.1f)) {
                objectRot.x = fmodf(objectRot.x, 360.f);
                objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                transform.SetEulerRot(objectRot);
            }
        
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            if (ImGui::DragFloat("##RotationY", &objectRot.y, 0.1f)) {
                objectRot.y = fmodf(objectRot.y, 360.f);
                objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                transform.SetEulerRot(objectRot);
            }
        
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            if (ImGui::DragFloat("##RotationZ", &objectRot.z, 0.1f)) {
                objectRot.z = fmodf(objectRot.z, 360.f);
                objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                transform.SetEulerRot(objectRot);
            }
        
            ImGui::EndTable();
        }
        
        if (!transform.IsCamera())
        {
            if (ImGui::BeginTable("ScaleTable", 7, ImGuiTableFlags_SizingFixedFit))
            {
                // Inputs for scale.
                Vector3 objectScale = transform.GetScale();
        
                ImGui::TableNextColumn();
                ImGui::Text("Scale");
                ImGui::TableNextColumn();
                ImGui::Dummy({17, 0}); ImGui::SameLine();
                ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
                ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
                ImGui::SetNextItemWidth(65.f);
                if (ImGui::DragFloat("##ScaleX", &objectScale.x, 0.1f)) {
                    transform.SetScale(objectScale);
                }
        
                ImGui::TableNextColumn();
                ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
                ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
                ImGui::SetNextItemWidth(65.f);
                if (ImGui::DragFloat("##ScaleY", &objectScale.y, 0.1f)) {
                    transform.SetScale(objectScale);
                }
        
                ImGui::TableNextColumn();
                ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
                ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
                ImGui::SetNextItemWidth(65.f);
                if (ImGui::DragFloat("##ScaleZ", &objectScale.z, 0.1f)) {
                    transform.SetScale(objectScale);
                }
        
                ImGui::EndTable();
            }
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
}

void Editor::Show2DTransformUi(Vector2& position, Vector2& scale, float& rotation, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[4]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Transform##2D" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        if (ImGui::BeginTable("PositionTable", 7, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextColumn();
            ImGui::Text("Position");
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            ImGui::DragFloat("##PositionX", &position.x, 0.005f);
        
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            ImGui::DragFloat("##PositionY", &position.y, 0.005f);
        
            ImGui::EndTable();
        }
        //
        // if (ImGui::BeginTable("RotationTable", 7, ImGuiTableFlags_SizingFixedFit))
        // {
        //     ImGui::TableNextColumn();
        //     ImGui::Text("Rotation");
        //     ImGui::TableNextColumn();
        //     ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
        //     ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
        //     ImGui::SetNextItemWidth(160.f);
        //     ImGui::DragFloat("##RotationZ", &rotation, 0.005f);
        //
        //     ImGui::EndTable();
        // }
        
        if (ImGui::BeginTable("ScaleTable", 7, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextColumn();
            ImGui::Text("Scale");
            ImGui::TableNextColumn();
            ImGui::Dummy({17, 0}); ImGui::SameLine();
            ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            ImGui::DragFloat("##ScaleX", &scale.x, 0.005f);
    
            ImGui::TableNextColumn();
            ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
            ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y-3});
            ImGui::SetNextItemWidth(65.f);
            ImGui::DragFloat("##ScaleY", &scale.y, 0.005f);
    
            ImGui::EndTable();
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
}

void Editor::ShowButtonWidgetUi(ButtonWidget* widget, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[6]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Properties##Widget" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Textures");
        
        ImGui::Text("Default");
        ShowTextureUi(widget->buttonTexture, 215);
        
        ImGui::Text("Hovered");
        ShowTextureUi(widget->hoverTexture,  215);
        
        ImGui::Text("Pressed");
        ShowTextureUi(widget->pressTexture,  215);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
}

void Editor::ShowTextWidgetUi(TextWidget* widget, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[6]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Properties##Widget" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        ImGui::Text("Text"); ImGui::SameLine(); ImGui::Dummy({ 3, 0 }); ImGui::SameLine();
        ImGui::InputText ("##Text",  &widget->text);

        ImGui::Text("Color"); ImGui::SameLine();
        ImGui::ColorEdit4("##Color", &widget->color.r, ImGuiColorEditFlags_DisplayHSV);

        const Font* currentFont = widget->font;
        
        ImGui::AlignTextToFramePadding(); ImGui::Text("Font"); ImGui::SameLine(); ImGui::Dummy({ 3, 0 }); ImGui::SameLine();
        if (ImGui::BeginCombo("##currTextFont", (widget->font ? currentFont->GetName().c_str() : "none")))
        {
            static std::vector<Font*> fonts = resourceManager->GetAll<Font>();
            
            {
                const bool isSelected = !widget->font;
                if (ImGui::Selectable("none", isSelected)) {
                    widget->font = nullptr;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            
            for (const auto& font : fonts)
            {
                const bool isSelected = (font->GetName() == (currentFont ? currentFont->GetName() : ""));
                if (ImGui::Selectable(font->GetName().c_str(), isSelected)) {
                    widget->font = font;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
}

void Editor::ShowTextureUi(const Texture* texture, const float& size, const bool& isClickable, const std::string& tooltip) const
{
    const Vector2 normalizedSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight());
    ImGui::Image(
        (ImTextureID)texture->GetId(), 
        { normalizedSize.x * size, normalizedSize.y * size }, 
        { 0, 1 }, { 1, 0 }
    );
    
    if (ImGui::IsItemHovered())
    {
        // if(isClickable && ImGui::IsItemClicked())
        // {
        //     std::cout << "ew" << std::endl;
        // }
        if (isClickable) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        if (!tooltip.empty() && ImGui::BeginTooltip()) {
            ImGui::Text("%s", tooltip.c_str());
            ImGui::EndTooltip();
        }
    }
}

void Editor::ShowMaterialUi(Material* material, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[8]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Material##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        if (ImGui::TreeNode("Colors"))
        {
            ImGui::PushItemWidth(200);
            ImGui::Text("Ambiant"); ImGui::SameLine(); ImGui::Dummy({31, 0}); ImGui::SameLine();
            ImGui::ColorEdit3("##Ambient",  material->ambient.ptr());

            ImGui::Text("Diffuse"); ImGui::SameLine(); ImGui::Dummy({31, 0}); ImGui::SameLine();
            ImGui::ColorEdit3("##Diffuse",  material->diffuse.ptr());

            ImGui::Text("Specular"); ImGui::SameLine(); ImGui::Dummy({25, 0}); ImGui::SameLine();
            ImGui::ColorEdit3("##Specular", material->specular.ptr());
            
            ImGui::Text("Emission"); ImGui::SameLine(); ImGui::Dummy({25, 0}); ImGui::SameLine();
            ImGui::ColorEdit3("##Emission", material->emission.ptr());

            ImGui::Text("Shininess"); ImGui::SameLine(); ImGui::Dummy({17, 0}); ImGui::SameLine();
            ImGui::DragFloat("##Shininess", &material->shininess, 1, 0, 100);

            ImGui::Text("Transparency"); ImGui::SameLine();
            ImGui::DragFloat("##Transparency", &material->transparency, 0.01f, 0, 1);
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(("Textures##" + id).c_str()))
        {
            Texture** materialTextures[] = { &material->ambientTexture, &material->diffuseTexture, &material->specularTexture, &material->emissionTexture, &material->shininessMap, &material->alphaMap, &material->normalMap };
            const char* materialTextureNames[] = { "Ambient texture", "Diffuse texture", "Specular texture", "Emission texture", "Shininess map", "Alpha map", "Normal map" };
            for (int i = 0; i < 7; i++)
            {
                // Add texture button.
                if (*materialTextures[i] == nullptr)
                {
                    if (ImGui::Button(("+##" + std::string(materialTextureNames[i])).c_str()))
                        *materialTextures[i] = resourceManager->Get<Texture>("Resources/Textures/defaultTexture.png");
                    ImGui::SameLine();
                    ImGui::AlignTextToFramePadding();
                    ImGui::TextWrapped("%s", materialTextureNames[i]);
                }
                // Show textures.
                else
                {
                    if (ImGui::TreeNode(materialTextureNames[i]))
                    {
                        ImGui::Unindent(5);
                        ShowTextureUi(*materialTextures[i], ImGui::GetWindowWidth() - 70, true, (*materialTextures[i])->GetName());
                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                        {
                            void** addressOfObject = new void*(*materialTextures[i]);
                            ImGui::SetDragDropPayload("TexturePayload", addressOfObject, sizeof(void**));
                            ImGui::Text("Move %s", (*materialTextures[i])->GetName().c_str());
                            ImGui::EndDragDropSource();
                            delete addressOfObject;
                        }
                        if (ImGui::BeginDragDropTarget())
                        {
                            // Enable dropping textures onto materials.
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TexturePayload"))
                            {
                                Assert(payload->DataSize == sizeof(materialTextures[i]), "Texture drag/drop payload of wrong size.");
                                Texture* droppedNode = *(Texture**)payload->Data;
                                *materialTextures[i] = droppedNode;
                            }
                            ImGui::EndDragDropTarget();
                        }
                        if (ImGui::Button("Remove texture"))
                        {
                            *materialTextures[i] = nullptr;
                        }
                        ImGui::Indent(5);
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::TreePop();
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
}

void Editor::ShowBoneUi(const Bone* bone) const
{
    if (bone->children.empty())
    {
        ImGui::BulletText("%s", bone->GetName().c_str());
    }
    else
    {
        if (ImGui::TreeNode(bone->GetName().c_str()))
        {
            for (const Bone* child : bone->children)
                ShowBoneUi(child);
            ImGui::TreePop();
        }
    }
}

void Editor::ShowAnimatorUI(Animator& animator, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[10]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Animator##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::PushItemWidth(215);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        Animation* currentAnimation = animator.GetCurrentAnimation();

        ImGui::AlignTextToFramePadding(); ImGui::Text("Animation"); ImGui::SameLine();
        if (ImGui::BeginCombo("##curAnimInput", (animator.IsAnimating() ? currentAnimation->name.c_str() : "none")))
        {
            // No animation.
            {
                const bool isSelected = !animator.IsAnimating();
                if (ImGui::Selectable("none", isSelected)/* && !animator.IsTransitioning()*/) {
                    animator.SetCurrentAnimation("");
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
        
            // All animations.
            for (auto& [name, anim] : animator.GetAllAnimations())
            {
                const bool isSelected = (name == (currentAnimation ? currentAnimation->name : ""));
                if (ImGui::Selectable(anim->name.c_str(), isSelected)/* && !animator.IsTransitioning()*/) {
                    animator.SetCurrentAnimation(name);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (currentAnimation)
        {
            ImGui::AlignTextToFramePadding();  ImGui::Text("Speed"); ImGui::SameLine(); ImGui::Dummy({25, 0}); ImGui::SameLine();
            if (ImGui::DragFloat("##Speed", &currentAnimation->speed, 0.5f, 0))
            {
                currentAnimation->speed = clampAbove(currentAnimation->speed, 0);
            }
            // ImGui::Checkbox("Reversed", &animation->reversed);
            ImGui::AlignTextToFramePadding();  ImGui::Text("Paused"); ImGui::SameLine(); ImGui::Dummy({17, 0}); ImGui::SameLine();
            ImGui::Checkbox("##Paused", &currentAnimation->paused);
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
        ImGui::PopItemWidth();
    }
}

void Editor::ShowMeshUi(const Mesh* mesh, const std::string& id) const
{
    auto texture = miscIcons.at(miscIconPaths[7]);
    auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Mesh##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
        ImGui::Text("Name: %s",          mesh->GetName().c_str());
        ImGui::Text("Vertex count: %zu", mesh->GetVertexCount());
        if (ImGui::TreeNode("Sub-meshes"))
        {
            for (const SubMesh* subMesh : mesh->subMeshes) 
            {
                if (ImGui::TreeNode(subMesh->GetName().c_str()))
                {
                    ImGui::Text("Vertex count: %d", subMesh->GetVertexCount());
                    ImGui::Text("Shader program: %s", subMesh->GetShaderProgram() ? subMesh->GetShaderProgram()->GetName().c_str() : "null");
                    ShowMaterialUi(subMesh->GetMaterial(), id);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
    
    if (mesh->WasSentToPipeline() && mesh->GetSkeleton())
    {
        texture = miscIcons.at(miscIconPaths[9]);
        textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
        
        if(ImGui::CollapsingHeaderWithIcon(("Skeleton##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
            ShowBoneUi(mesh->GetSkeleton()->GetRootBone());
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        }
        
        ShowAnimatorUI(mesh->GetSkeleton()->GetAnimator(), id);
    }
}

void Editor::ShowInstancedUi(InstancedObject* instancedObject, const std::string& id) const
{
    ShowMeshUi(instancedObject->mesh, id);

    const auto texture = miscIcons.at(miscIconPaths[18  ]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if (ImGui::CollapsingHeaderWithIcon(("Transforms##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap)) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);

        static int selectedTransform = 0;
        ImGui::AlignTextToFramePadding(); ImGui::Text("Selected index"); ImGui::SameLine();
        if(ImGui::DragInt("##InstanceTransform", &selectedTransform, 0.5f, 0, (int)instancedObject->transforms.size() - 1)) {
            clamp(selectedTransform, 0, instancedObject->transforms.size() - 1);
        }   
        // ImGui::Dummy({20, 0}); ImGui::SameLine();   
        ShowTransformUi(instancedObject->transforms.at(selectedTransform), "##Instanced" + id);
    }
}

void Editor::ShowLightUi(Light* light, const std::string& id) const
{
    auto texture = miscIcons.at(miscIconPaths[5]);
    auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    // KeyBindings floats for light parameters.
    if(ImGui::CollapsingHeaderWithIcon(("Colors##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        ImGui::AlignTextToFramePadding(); ImGui::Text("Ambient"); ImGui::SameLine(); ImGui::Dummy({1, 0}); ImGui::SameLine();
        ImGui::ColorEdit3("##Ambient", light->ambient.ptr());

        ImGui::AlignTextToFramePadding(); ImGui::Text("Diffuse"); ImGui::SameLine(); ImGui::Dummy({1, 0}); ImGui::SameLine();
        ImGui::ColorEdit3("##Diffuse", light->diffuse.ptr());

        ImGui::AlignTextToFramePadding(); ImGui::Text("Specular"); ImGui::SameLine();
        ImGui::ColorEdit3("##Specular", light->specular.ptr());
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
    
    texture = miscIcons.at(miscIconPaths[6]);
    textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Properties##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        ImGui::AlignTextToFramePadding(); ImGui::Text("Constant"); ImGui::SameLine(); ImGui::Dummy({2, 0}); ImGui::SameLine();
        ImGui::DragFloat("##Constant", &light->constant, 0.05f);

        ImGui::AlignTextToFramePadding(); ImGui::Text("Linear"); ImGui::SameLine(); ImGui::Dummy({17, 0}); ImGui::SameLine();
        ImGui::DragFloat("##Linear", &light->linear,   0.05f);

        ImGui::AlignTextToFramePadding(); ImGui::Text("Quadratic"); ImGui::SameLine();
        ImGui::DragFloat("##Quadratic", &light->quadratic, 0.05f);
        
        ImGui::NewLine();

        ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Cutoffs");
        ImGui::AlignTextToFramePadding(); ImGui::Text("Outer"); ImGui::SameLine();
        ImGui::SliderAngle("##Outer", &light->outerCutoff, 0, 360);
        
        ImGui::AlignTextToFramePadding(); ImGui::Text("Inner"); ImGui::SameLine();
        ImGui::SliderAngle("##Inner", &light->innerCutoff, 0, 360);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()+5);
    }
}

void Editor::ShowCameraUi(Camera* camera, const std::string& id) const
{
    const auto texture = miscIcons.at(miscIconPaths[6]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    // KeyBindings floats for camera parameters.
    if(ImGui::CollapsingHeaderWithIcon(("Properties##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        CameraParams params = camera->GetParams();
        float fov = params.fov, near = params.near, far = params.far;
        ImGui::PushItemWidth(215);
        
        ImGui::Text("Field of view"); ImGui::SameLine();
        ImGui::DragFloat("##Field of view", &fov,  0.2f);
        
        ImGui::Text("Near"); ImGui::SameLine(); ImGui::Dummy({64, 0}); ImGui::SameLine();
        ImGui::DragFloat("##Near", &near, 0.1f);
        
        ImGui::Text("Far"); ImGui::SameLine(); ImGui::Dummy({71, 0}); ImGui::SameLine();
        ImGui::DragFloat("##Far", &far,  0.1f);
        
        ImGui::PopItemWidth();
        if (params.fov != fov || params.near != near || params.far != far)
        {
            params.fov = fov; params.near = near; params.far = far;
            camera->SetParams(params);
        }
    
        // Set as scene camera button.
        if (const Scene* scene = sceneManager->GetActiveScene(); CameraManager* cameraManager = scene->GetCameraManager())
        {
            if (cameraManager->GetSceneCamera() != camera)
            {
                if (ImGui::Button("Use this camera to render the scene"))
                    cameraManager->SetSceneCamera(camera);
            }
            else
            {
                ImGui::Text("This camera renders the scene");
            }
        }
    }
}

void Editor::ShowShaderDropArea()
{
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);

    const ImGuiContext& context = *GImGui;
    const ImGuiWindow* imguiWindow = context.CurrentWindow;
    ImDrawList* drawList = imguiWindow->DrawList;

    const ImVec2 size = ImVec2(ImGui::GetWindowWidth() - imGuiStyle->WindowPadding.x * 2 - 0.5f, 80.f);
    const ImVec2 min = ImVec2(imguiWindow->DC.CursorPos.x, imguiWindow->DC.CursorPos.y);
    const ImVec2 max = ImVec2(min.x + size.x, min.y + size.y);

    ImGui::RenderFrame(min, max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, imGuiStyle->FrameRounding);

    // Grey outline
    drawList->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_BorderShadow), imGuiStyle->FrameRounding, 0, 2);

    if (ImGui::BeginDragDropTargetCustom(ImRect(min, max), ImGui::GetID("##ShaderDragDropTarget")))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File"))
        {
            Assert(payload->DataSize == sizeof(void*), "File drag/drop payload of wrong size.");
            const fs::path* droppedFile = *(fs::path**)payload->Data;
            const fs::path extension = droppedFile->extension();

            if (extension == ".frag") {
                std::string path = droppedFile->string();
                std::replace(path.begin(), path.end(), '\\', '/');
                postProcessor->AddShader(new FragmentShader(path));
            }
            else {
                DebugLogWarning("File is not supported by the shader interface!");
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::SetCursorPos(ImVec2(size.x * 0.5f - 25.f, ImGui::GetCursorPos().y + ImGui::GetTextLineHeight() * 0.5f));
    ShowTextureUi(miscIcons[miscIconPaths[17]], 50.f);
    ImGui::SetCursorPos(ImVec2(size.x * 0.5f - 85.f, ImGui::GetCursorPos().y + ImGui::GetTextLineHeight() * 0.5f - 15.f));
    ImGui::TextColored({ 0.502f, 0.431f, 0.475f, 1.f }, "Drop a script file here.");
   
}

void Editor::ShowScriptDropArea()
{
    
    const ImGuiContext& context = *GImGui;
    const ImGuiWindow* imguiWindow = context.CurrentWindow;
    ImDrawList* drawList = imguiWindow->DrawList;

    const ImVec2 size = ImVec2(ImGui::GetWindowWidth() - imGuiStyle->WindowPadding.x * 2 - 0.5f, 80.f);
    const ImVec2 min = ImVec2(imguiWindow->DC.CursorPos.x, imguiWindow->DC.CursorPos.y);
    const ImVec2 max = ImVec2(min.x + size.x, min.y + size.y);

    ImGui::RenderFrame(min, max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, imGuiStyle->FrameRounding);

    // Grey outline
    drawList->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_BorderShadow), imGuiStyle->FrameRounding, 0, 2);

    if (ImGui::BeginDragDropTargetCustom(ImRect(min, max), ImGui::GetID("##ScriptDragDropTarget")))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File"))
        {
            Assert(payload->DataSize == sizeof(void*), "File drag/drop payload of wrong size.");
            const fs::path* droppedFile = *(fs::path**)payload->Data;
            const fs::path extension = droppedFile->extension();
        
            if (extension == ".lua") {
                selectedGameObject->AddScript(new LuaScript(droppedFile->string()));
            } else if (extension == ".py")  {
                selectedGameObject->AddScript(new PythonScript(droppedFile->string()));
            } else {
                DebugLogWarning("File is not supported by the scripting interface!");
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::SetCursorPos(ImVec2(size.x * 0.5f - 25.f, ImGui::GetCursorPos().y + ImGui::GetTextLineHeight()*0.5f));
    ShowTextureUi(miscIcons[miscIconPaths[17]], 50.f);
    ImGui::SetCursorPos(ImVec2(size.x * 0.5f - 85.f, ImGui::GetCursorPos().y + ImGui::GetTextLineHeight()*0.5f - 15.f));
    ImGui::TextColored({ 0.502f, 0.431f, 0.475f, 1.f }, "Drop a script file here.");
}

void Editor::ShowScriptUI(const std::string& id)
{
    const auto texture = miscIcons.at(miscIconPaths[20]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;

    const std::vector<ObjectScript*> loadedScripts = selectedGameObject->GetScripts();
    if(ImGui::CollapsingHeaderWithIcon(("Scripts##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY()-5);
        if (ImGui::BeginTable("LoadedScriptTable", loadedScripts.size() + 2, ImGuiTableFlags_SizingFixedFit)) {
            for (size_t i = 0; i < loadedScripts.size(); ++i) {
                const ObjectScript* currentScript = loadedScripts.at(i);
                const size_t archId = currentScript->GetArchId();
                std::string name = rfk::getDatabase().getArchetypeById(archId)->getName();
                
                const Texture* scriptTexture = nullptr;
                if (archId == PythonScript::staticGetArchetype().getId()) {
                    name = std::filesystem::path(((PythonScript*)currentScript)->GetName()).stem().string();
                    scriptTexture = scriptingIcons[scriptingIconPaths[1]];
                } else if(archId == LuaScript::staticGetArchetype().getId()) {
                    name = std::filesystem::path(((LuaScript*)currentScript)->GetName()).stem().string();
                    scriptTexture = scriptingIcons[scriptingIconPaths[2]];
                } else {
                    scriptTexture = scriptingIcons[scriptingIconPaths[0]];
                }

                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(scriptTexture, 25);
                
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                
                ImGui::Text(name.c_str());

                ImGui::TableNextColumn();
                if (ImGui::Button(("Remove##" + std::to_string(i)).c_str())) {
                    selectedGameObject->RemoveScript(currentScript);
                }
            }
            ImGui::EndTable();
        }
        ShowScriptDropArea();
        ImGui::Dummy({ 0, 5 });

        if (ImGui::Button("Create script", { ImGui::GetWindowWidth() - imGuiStyle->WindowPadding.x * 2 - 0.5f, 40 }))
        {
            const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 1.5f));
            if (!ImGui::IsPopupOpen("Create script")) ImGui::OpenPopup("Create script");
        }

        ImGui::Dummy({ 0, 5 });
    }

    if (ImGui::BeginPopupModal("Create script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static ScriptType scriptType = Undefined;
        ImGui::PushItemWidth(120 * 2 + 5);

        // Name input.
        static std::string scriptName; if (scriptName.empty()) scriptName = "DefaultName";
        ImGui::InputText("Name", &scriptName);

        // Lambdas to reset the popup and create the script object.
        const std::function resetPopup = [&] { ImGui::CloseCurrentPopup(); scriptType = Undefined; scriptName = "DefaultName"; };
        const std::function canCreate = [&] { return !scriptName.empty() && scriptType != Undefined; };
        const std::function createScript = [&]
        {
            bool foundScriptWithSameName = false;
            for (size_t i = 0; i < loadedScripts.size(); ++i)
            {
                const ObjectScript* currentScript = loadedScripts.at(i);
                const size_t archId = currentScript->GetArchId();
                std::string name = rfk::getDatabase().getArchetypeById(archId)->getName();
                
                if (archId == PythonScript::staticGetArchetype().getId()) {
                    name = std::filesystem::path(((PythonScript*)currentScript)->GetName()).filename().string();
                } else if(archId == LuaScript::staticGetArchetype().getId()) {
                    name = std::filesystem::path(((LuaScript*)currentScript)->GetName()).filename().string();
                }
                
                if(name == scriptName + (scriptType == Lua ? ".lua" : ".py")) {
                    std::cout << name << std::endl;
                    std::cout << scriptName << std::endl;
                    
                    foundScriptWithSameName = true;
                    resetPopup();
                }
            }

            if(!foundScriptWithSameName)
            {
                switch (scriptType)
                {
                case Lua: {
                    const std::string file = LuaScript::CreateLuaFile(scriptName);
                    if(file.empty())
                    {
                        DebugLogWarning("Script creation failed, filename already exist.");
                        break;
                    }
                    selectedGameObject->AddScript(new LuaScript( file));
                    Utils::WindowsUtils::OpenVisualStudio(absolute(std::filesystem::path(file)));
                    break; 
                }
                case Python: {
                    const std::string file = PythonScript::CreatePyFile(scriptName);
                    if(file.empty())
                    {
                        DebugLogWarning("Script creation failed, filename already exist.");
                        break;
                    }
                    selectedGameObject->AddScript(new PythonScript(file));
                    Utils::WindowsUtils::OpenVisualStudio(absolute(std::filesystem::path(file)));
                    break; 
                }
                default:
                    break;
                }
            }
        };
        
        if (ImGui::BeginCombo("Language", scriptType != Undefined ? ScriptTypeToString(scriptType).c_str() : "None"))
        {
            {
                const bool isSelected = scriptType == Undefined;
                if (ImGui::Selectable("None", isSelected)) {
                    scriptType = Undefined;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            
            for (const auto& type : AllScriptTypes)
            {
                const bool isSelected = scriptType != Undefined && type == scriptType;
                if (ImGui::Selectable(ScriptTypeToString(type).c_str(), isSelected)) {
                    scriptType = type;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Cancel", { 120, 0 }))
        {
            resetPopup();
        }
        ImGui::SameLine();
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !canCreate());
        if (ImGui::Button("Create", { 120, 0 }))
        {
            createScript();
            resetPopup();
        }
        ImGui::PopItemFlag();

        ImGui::PopItemWidth();
        ImGui::EndPopup();
    }
}

void Editor::ShowCubemapUi(Cubemap* cubemap, const std::string& id) const
{
    static const char* cubemapSideNames[6] =
    {
        "Right",
        "Left",
        "Top",
        "Bottom",
        "Front",
        "Back",
    };

    const auto texture = miscIcons.at(miscIconPaths[6]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    if(ImGui::CollapsingHeaderWithIcon(("Properties##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
        for (int i = 0; i < 6; i++)
        {
            if (ImGui::TreeNode(cubemapSideNames[i]))
            {
                const std::string textureName = cubemap->sides[i];
                ShowTextureUi(resourceManager->Get<Texture>(textureName), ImGui::GetWindowWidth() - 70, false, textureName);
                ImGui::TreePop();
            }
        }
    }
}

void Editor::ShowWidgetTreeNode(Widget* widget)
{
    const bool isSelected        = selectedWidget && selectedWidget->GetId() == widget->GetId();
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    
    if (isSelected)                     nodeFlags |= ImGuiTreeNodeFlags_Selected;
    if (widget->GetChildren().empty())  nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    const Texture* texture;
    switch (widget->GetType()) {
    case WidgetType::Text:
        texture = graphIcons[graphIconPaths[9]]; break;
    case WidgetType::Button:
        texture = graphIcons[graphIconPaths[10]]; break;
    case WidgetType::Empty:
    default:
        texture = graphIcons[graphIconPaths[0]]; break;
    }

    const bool isRoot = widget->name == "Root";
    if(isRoot) texture = graphIcons[graphIconPaths[8]];
    
    ShowTextureUi(texture, 25);
    ImGui::SameLine(); ImGui::AlignTextToFramePadding();
    if(isRoot) ImGui::SetNextItemOpen(true);
    const bool treeNodeOpen = ImGui::TreeNodeEx((widget->name + "##" + std::to_string(widget->GetId())).c_str(), nodeFlags);
    bool breakProcess  = false;

    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) && !ImGui::IsItemToggledOpen() && widget->GetId() > 0)
        selectedWidget = widget;

    // Draw children Ui.
    if (treeNodeOpen) {
        if (!breakProcess) {
            for (Widget* child : widget->GetChildren()) {
                ShowWidgetTreeNode(child);
            }
        }
        ImGui::TreePop();
    }
}

void Editor::ShowObjectTreeNode(GameObject* object)
{
    const bool isSelected        = selectedGameObject != nullptr && selectedGameObject->GetId() == object->GetId();
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    
    if (isSelected)                    nodeFlags |= ImGuiTreeNodeFlags_Selected;
    if (object->GetChildren().empty()) nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    const Texture* texture;
    switch (object->GetType()) {
    case GameObjectType::Model:
        texture = graphIcons[graphIconPaths[1]]; break;
    case GameObjectType::Instanced:
        texture = graphIcons[graphIconPaths[2]]; break;
    case GameObjectType::Camera:
        texture = graphIcons[graphIconPaths[3]]; break;
    case GameObjectType::Cubemap:
        texture = graphIcons[graphIconPaths[4]]; break;
    case GameObjectType::Sound:
        texture = graphIcons[graphIconPaths[5]]; break;
    case GameObjectType::Canvas:
        texture = graphIcons[graphIconPaths[6]]; break;
    case GameObjectType::Light:
        texture = graphIcons[graphIconPaths[7]]; break;
    case GameObjectType::Empty:
    default:
        texture = graphIcons[graphIconPaths[0]]; break;
    }

    const bool isRoot = object->name == "Root";
    if(isRoot) texture = graphIcons[graphIconPaths[8]];
    
    ShowTextureUi(texture, 25);
    ImGui::SameLine(); ImGui::AlignTextToFramePadding();
    const bool treeNodeOpen = ImGui::TreeNodeEx((object->name + "##" + std::to_string(object->GetId())).c_str(), nodeFlags);
    bool breakProcess = false;
    
    // Enable dragging tree nodes.
    if (object->GetId() > 0 && object->GetParent() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        void** addressOfObject = new void*(object);
        ImGui::SetDragDropPayload("GameObject", addressOfObject, sizeof(void**));
        ImGui::Text("Move %s", object->name.c_str());
        ImGui::EndDragDropSource();
        delete addressOfObject;
    }

    // Enable dropping tree nodes.
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
        {
            Assert(payload->DataSize == sizeof(void**), "Scene node drag/drop payload of wrong size.");
            GameObject* droppedObject = *(GameObject**)payload->Data;
            if (droppedObject->SetParent(object))
                breakProcess = true;
        }
        ImGui::EndDragDropTarget();
    }
    
    // Select the game object if the tree node was clicked.
    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) && !ImGui::IsItemToggledOpen() && object->GetId() > 0)
        selectedGameObject = object;
    
    // Draw children Ui.
    if (treeNodeOpen) {
        if (!breakProcess) {
            for (GameObject* child : object->GetChildren()) {
                ShowObjectTreeNode(child);
            }
        }
        ImGui::TreePop();
    }
}

void Editor::ShowCanvasGraphPopup()
{
    static std::optional<WidgetType> newWidgetType = {};

    bool renameButtonPressed = false;
    if (ImGui::BeginPopup("CanvasGraphPopup", ImGuiWindowFlags_NoMove))
    {
        if (ImGui::BeginTable("AddWidgetTable", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ShowTextureUi(miscIcons[miscIconPaths[0]], 25);
            ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
            
            if(ImGui::BeginMenu("Add Widget"))
            {
                if (ImGui::BeginTable("EmptyWidgetTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(widgetIcons[widgetIconPaths[(int)WidgetType::Empty]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y+3});
                    if(ImGui::MenuItem("Empty")) newWidgetType = WidgetType::Empty;
                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("TextWidgetTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(widgetIcons[widgetIconPaths[(int)WidgetType::Text]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y+3});
                    if(ImGui::MenuItem("Text")) newWidgetType = WidgetType::Text;
                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("ButtonWidgetTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(widgetIcons[widgetIconPaths[(int)WidgetType::Button]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y+3});
                    if(ImGui::MenuItem("Button")) newWidgetType = WidgetType::Button;
                    ImGui::EndTable();
                }
                ImGui::EndMenu();
            }

            if (selectedWidget)
            {
                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[12]], 25);
                ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
                if (ImGui::MenuItem("Rename"))
                {
                    renameButtonPressed = true;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[11]], 25);
                ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
                if (ImGui::MenuItem("Remove"))
                {
                    selectedCanvas->RemoveWidget(selectedWidget);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndPopup();
    }

    if (selectedWidget)
    {
        if (renameButtonPressed)
            ImGui::OpenPopup("WidgetRenamePopup");
        
        ShowRenamePopup("WidgetRenamePopup", selectedWidget->name.c_str(),
            [this](std::string newName) { selectedWidget->name = std::move(newName); }
        );
    }
    
    if (newWidgetType.has_value())
    {
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 1.5f));
        const std::string popupName = "Add " + WidgetTypeToString(newWidgetType.value()) + " Object";
        if (!ImGui::IsPopupOpen(popupName.c_str())) ImGui::OpenPopup(popupName.c_str());
        if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::PushItemWidth(120*2+5);

            const ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPos({ cursorPos.x - 20, cursorPos.y - 20 });
            ShowTextureUi(objectIcons[objectIconPaths[(int)newWidgetType.value()]], 15);
            ImGui::SetCursorPos(cursorPos);

            // Name input.
            static std::string objectName; if (objectName.empty()) objectName = WidgetTypeToString(newWidgetType.value());
            ImGui::InputText("Name", &objectName);

            // Lambdas to reset the popup and create the game object.
            const std::function resetPopup   = [&]{ newWidgetType = {}; objectName = ""; ImGui::CloseCurrentPopup(); };
            const std::function canCreate    = [&]{ return !objectName.empty(); };
            const std::function createObject = [&]{
                if (newWidgetType == WidgetType::Text) {
                    selectedCanvas->AddTextWidget(objectName, nullptr);
                } else if (newWidgetType == WidgetType::Button) {
                    selectedCanvas->AddButtonWidget(objectName, nullptr);
                } else {
                    selectedCanvas->AddEmptyWidget(objectName);
                }
            };
            
            if (ImGui::Button("Cancel", { 120, 0 }))
            {
                resetPopup();
            }
            ImGui::SameLine();
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !canCreate());
            if (ImGui::Button("Create", { 120, 0 }))
            {
                createObject();
                resetPopup();
            }
            ImGui::PopItemFlag();
            
            ImGui::PopItemWidth();
            ImGui::EndPopup();
        }
    }
}

void Editor::ShowCreateScenePopup()
{
    if (ImGui::BeginPopupModal("Create scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::PushItemWidth(120 * 2 + 5);

        // Name input.
        static std::string sceneName; if (sceneName.empty()) sceneName = "DefaultSceneName";
        ImGui::InputText("Name", &sceneName);

        const std::function resetPopup = [&] { ImGui::CloseCurrentPopup(); sceneName = "DefaultSceneName"; };
        const std::function canCreate = [&] { return !sceneName.empty(); };
        const std::function createScene = [&]
        {
            Scene* scene = sceneManager->CreateDefault(sceneName);
            scene->Load();
            sceneManager->SetActiveScene(scene->GetName(), false);
            DebugLogInfo("Successfully created scene.");
        };
        
        if (ImGui::Button("Cancel", { 120, 0 }))
        {
            resetPopup();
        }
        ImGui::SameLine();
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !canCreate());
        if (ImGui::Button("Create", { 120, 0 }))
        {
            createScene();
            resetPopup();
        }
        ImGui::PopItemFlag();

        ImGui::PopItemWidth();
        ImGui::EndPopup();
    }
}

void Editor::ShowPostProcessWindow()
{
    if (ImGui::Begin("Post Process", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        const std::vector<ShaderProgram*> loadedShaders = postProcessor->GetShaders();
        if (ImGui::BeginTable("LoadedPostProcessTable", (int)loadedShaders.size() + 2, ImGuiTableFlags_SizingFixedFit)) {
            for (size_t i = 0; i < loadedShaders.size(); ++i) {
                const ShaderProgram* currentShader = loadedShaders.at(i);
                
                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::BulletText(currentShader->GetName().c_str());

                ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[19]], 25);

                ImGui::TableNextColumn();
                if (ImGui::Button(("Unload##" + std::to_string(i)).c_str())) {
                    postProcessor->RemoveShaderProgram(currentShader);
                }
            }
            ImGui::EndTable();
            ImGui::Dummy({ 0, 5 });
        }
        ShowShaderDropArea();
        ImGui::Dummy({ 0, 5 });
    }
    ImGui::End();
}

void Editor::ShowStatsWindow() const
{
    if (ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("FPS: %.0f", engine->GetTimeManager()->FPS());
        ImGui::Text("Delta Time: %f", engine->GetTimeManager()->DeltaTime());
        ImGui::Text("Mouse: X:%.0f Y:%.0f", window->GetInputs().mousePos.x, window->GetInputs().mousePos.y);
    }
    ImGui::End();
}

void Editor::ShowSettingsWindow() const
{
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        int targetFps = engine->GetTimeManager()->GetTargetFPS();
        ImGui::AlignTextToFramePadding(); ImGui::Text("Target FPS:"); ImGui::SameLine();
        ImGui::SetNextItemWidth(55);
        if (ImGui::DragInt("##targetFPS", &targetFps, 0.1f, 15) && targetFps >= 15)
            engine->GetTimeManager()->SetTargetFPS(targetFps);


        static bool vsync = false;
        ImGui::AlignTextToFramePadding(); ImGui::Text("VSync:"); ImGui::SameLine(); ImGui::Dummy({32, 0}); ImGui::SameLine();
        if (ImGui::Checkbox("##VSync", &vsync))
            window->SetVsync(vsync);
    }
    ImGui::End();
    
}

void Editor::ShowSceneGraphPopup()
{
    static std::optional<GameObjectType> newObjectType = {};

    bool renameButtonPressed = false;
    if (ImGui::BeginPopup("SceneGraphPopup", ImGuiWindowFlags_NoMove))
    {
        if (ImGui::BeginTable("AddObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ShowTextureUi(miscIcons[miscIconPaths[0]], 25);
            ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
            
            if(ImGui::BeginMenu("Add Object"))
            {
                if (ImGui::BeginTable("EmptyObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Empty]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Empty")) newObjectType = GameObjectType::Empty;
                    ImGui::EndTable();
                }
                
                if (ImGui::BeginTable("ModelObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Model]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Model")) newObjectType = GameObjectType::Model;
                    ImGui::EndTable();
                }
                
                if (ImGui::BeginTable("InstancedObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Instanced]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Instanced")) newObjectType = GameObjectType::Instanced;
                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("CameraObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Camera]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Camera")) newObjectType = GameObjectType::Camera;
                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("LightObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Light+1]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Light")) newObjectType = GameObjectType::Light;
                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("CanvasObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Canvas]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Canvas")) newObjectType = GameObjectType::Canvas;
                    ImGui::EndTable();
                }
                
                if (ImGui::BeginTable("SoundObjectTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();
                    ShowTextureUi(objectIcons[objectIconPaths[(int)GameObjectType::Sound]], 25);
                    ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                    if(ImGui::MenuItem("Sound")) newObjectType = GameObjectType::Sound;
                    ImGui::EndTable();
                }
                ImGui::EndMenu();
            }

            if (selectedGameObject)
            {
                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[12]], 25);
                ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
                if (ImGui::MenuItem("Rename"))
                {
                    renameButtonPressed = true;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::TableNextRow(); ImGui::TableNextColumn();
                ShowTextureUi(miscIcons[miscIconPaths[11]], 25);
                ImGui::TableNextColumn(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
                
                if (ImGui::MenuItem("Remove"))
                {
                    sceneManager->GetActiveScene()->GetSceneGraph()->Remove(selectedGameObject);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndPopup();
    }

    // Game object rename popup.
    if (selectedGameObject)
    {
        if (renameButtonPressed || ImGui::IsKeyPressed(ImGuiKey_F2))
            ImGui::OpenPopup("GameObjectRenamePopup");

        if (ImGui::IsKeyPressed(ImGuiKey_Delete))
            sceneManager->GetActiveScene()->GetSceneGraph()->Remove(selectedGameObject);
        
        ShowRenamePopup("GameObjectRenamePopup", selectedGameObject->name.c_str(),
            [this](std::string newName) { selectedGameObject->name = std::move(newName); }
        );
    }

    // Game object creation popup.
    if (newObjectType.has_value())
    {
        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 1.5f));
        const std::string popupName = "Add " + GameObjectTypeToString(newObjectType.value()) + " Object";
        if (!ImGui::IsPopupOpen(popupName.c_str())) ImGui::OpenPopup(popupName.c_str());
        if (ImGui::BeginPopupModal(popupName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::PushItemWidth(120*2+5);

            const ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPos({ cursorPos.x - 20, cursorPos.y - 20 });
            ShowTextureUi(objectIcons[objectIconPaths[(int)newObjectType.value()]], 15);
            ImGui::SetCursorPos(cursorPos);

            // Name input.
            static std::string objectName; if (objectName.empty()) objectName = GameObjectTypeToString(newObjectType.value());
            ImGui::InputText("Name", &objectName);

            // Object creation data.
            union ObjectCreationData
            {
                Mesh*    mesh;
                Cubemap* cubemap;
                Sound* sound;
            };
            static ObjectCreationData creationData;
            static int                instanceCount = 1;

            // Lambdas to reset the popup and create the game object.
            std::function resetPopup   = [&]{ newObjectType = {}; objectName = ""; creationData = {}; ImGui::CloseCurrentPopup(); };
            std::function canCreate    = [&]{ return !objectName.empty(); };
            std::function createObject = [&]{
                if (const Scene* scene = sceneManager->GetActiveScene())
                    scene->GetSceneGraph()->AddEmpty(objectName);
            };

            switch (newObjectType.value())
            {
            case GameObjectType::Model:
            case GameObjectType::Instanced:
                {
                    // Input to select mesh.
                    static std::vector<Mesh*> meshes; if (meshes.empty()) meshes = resourceManager->GetAll<Mesh>();
                    if (ImGui::BeginCombo("Mesh", creationData.mesh ? creationData.mesh->GetName().c_str() : "None"))
                    {
                        {
                            const bool isSelected = !creationData.mesh;
                            if (ImGui::Selectable("None", isSelected)) {
                                creationData.mesh = nullptr;
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        for (Mesh* mesh : meshes)
                        {
                            const bool isSelected = creationData.mesh && (mesh->GetName() == creationData.mesh->GetName());
                            if (ImGui::Selectable(mesh->GetName().c_str(), isSelected)) {
                                creationData.mesh = mesh;
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    // Input for instance count.
                    if (newObjectType.value() == GameObjectType::Instanced && ImGui::DragInt("Count", &instanceCount, 0.5f) && instanceCount < 1)
                        instanceCount = 1;
                    
                    // Set lambdas.
                    resetPopup   = [&]{ newObjectType = {}; objectName = ""; creationData = {}; meshes = {}; ImGui::CloseCurrentPopup(); };
                    canCreate    = [&]{ return !objectName.empty() && creationData.mesh; };
                    if (newObjectType.value() == GameObjectType::Instanced)
                    {
                        createObject = [&]{
                            if (const Scene* scene = sceneManager->GetActiveScene())
                                scene->GetSceneGraph()->AddInstanced(objectName, creationData.mesh, instanceCount);
                        };
                    }
                    else
                    {
                        createObject = [&]{
                            if (const Scene* scene = sceneManager->GetActiveScene())
                                scene->GetSceneGraph()->AddModel(objectName, creationData.mesh);
                        };
                    }
                    break;
                }
            case GameObjectType::Cubemap:
                {
                    // Input to select mesh.
                    static std::vector<Cubemap*> cubemaps; if (cubemaps.empty()) cubemaps = resourceManager->GetAll<Cubemap>();
                    if (ImGui::BeginCombo("Cubemap", creationData.cubemap ? creationData.cubemap->GetName().c_str() : "None"))
                    {
                        {
                            const bool isSelected = !creationData.cubemap;
                            if (ImGui::Selectable("None", isSelected)) {
                                creationData.cubemap = nullptr;
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        for (Cubemap* cubemap : cubemaps)
                        {
                            const bool isSelected = creationData.cubemap && (cubemap->GetName() == creationData.cubemap->GetName());
                            if (ImGui::Selectable(cubemap->GetName().c_str(), isSelected)) {
                                creationData.cubemap = cubemap;
                            }
                            if (isSelected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    // Set lambdas.
                    resetPopup   = [&]{ newObjectType = {}; objectName = ""; creationData = {}; cubemaps = {}; ImGui::CloseCurrentPopup(); };
                    canCreate    = [&]{ return !objectName.empty() && creationData.cubemap; };
                    createObject = [&]{
                        if (const Scene* scene = sceneManager->GetActiveScene())
                            scene->GetSceneGraph()->AddCubemap(objectName, creationData.cubemap);
                    };
                    break;
                }
            case GameObjectType::Camera:
                createObject = [&]{
                    if (const Scene* scene = sceneManager->GetActiveScene())
                        scene->GetSceneGraph()->AddCamera(objectName, scene->GetCameraManager()->Create({ (unsigned int)window->GetWidth(), (unsigned int)window->GetHeight() }));
                };
                break;
            case GameObjectType::Light:
                createObject = [&]{
                    if (const Scene* scene = sceneManager->GetActiveScene())
                        scene->GetSceneGraph()->AddLight(objectName, scene->GetLightManager()->Create());
                };
                break;
            case GameObjectType::Sound:
                createObject = [&]{
                    if (const Scene* scene = sceneManager->GetActiveScene())
                        scene->GetSceneGraph()->AddSound(objectName);
                };
                break;
            case GameObjectType::Canvas:
                createObject = [&]{
                    if (const Scene* scene = sceneManager->GetActiveScene())
                        scene->GetSceneGraph()->AddCanvas(objectName);
                };
                break;
            case GameObjectType::Empty:
            default:
                break;
            }
            
            if (ImGui::Button("Cancel", { 120, 0 }))
            {
                resetPopup();
            }
            ImGui::SameLine();
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !canCreate());
            if (ImGui::Button("Create", { 120, 0 }))
            {
                createObject();
                resetPopup();
            }
            ImGui::PopItemFlag();
            
            ImGui::PopItemWidth();
            ImGui::EndPopup();
        }
    }
}

FileIcon Editor::GetFileIcon(const std::filesystem::path& path) const
{
    FileIcon fileIcon = FileIcon::Folder;
    if (path.has_extension())
    {
        // Show a different icon for each supported file type.
        bool extensionFound = false;
        for (size_t i = 0; i < fileExtensions.size(); i++)
        {
            if (std::find(fileExtensions[i].begin(), fileExtensions[i].end(), path.extension()) != fileExtensions[i].end())
            {
                fileIcon = (FileIcon)((size_t)FileIcon::Code + i);
                extensionFound = true;
                break;
            }
        }
        
        // Show a default document icon for unsupported file types.
        if (!extensionFound) {
            fileIcon = FileIcon::Document;
        }
    }
    return fileIcon;
}

void Editor::ShowFileUi(const fs::path& path, const float& xMax)
{
    const std::string filename = path.filename().string();
    const ImVec2      textSize = ImGui::CalcTextSize(filename.c_str());
    
    if (ImGui::BeginChild(("file_" + filename).c_str(), { fileIconsSize, fileIconsSize + imGuiStyle->WindowPadding.y + textSize.y }))
    {
        // Show file icon.
        ShowTextureUi(fileIcons[fileIconPaths[(size_t)GetFileIcon(path)]], fileIconsSize, true, (textSize.x < fileIconsSize ? "" : filename));

        // If this path points to a folder, let the user double click it to navigate to its contents.
        if (ImGui::IsItemHovered())
        {
            if (!path.has_extension() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                explorerPath = path;
            else if (path.has_extension() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                selectedFile = path;
        }

        // Enable dragging files and folders.
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            const void** draggedFile = new const void*(&path);
            ImGui::SetDragDropPayload("File", draggedFile, sizeof(void*));
            ImGui::Text("Move %s", filename.c_str());
            ImGui::EndDragDropSource();
            delete draggedFile;
        }

        // Enable dropping files and folders into other folders.
        if (!path.has_extension() && ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File"))
            {
                Assert(payload->DataSize == sizeof(void*), "File drag/drop payload of wrong size.");
                const fs::path* droppedFile = *(fs::path**)payload->Data;
                fs::rename(*droppedFile, path / droppedFile->filename());
                shouldReloadExplorer = true;
            }
            ImGui::EndDragDropTarget();
        }

        bool renameButtonPressed = false, deleteButtonPressed = false;
        if(ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Right))
            ImGui::OpenPopup("FilePopup");
        
        ShowFilePopup("FilePopup", path, renameButtonPressed, deleteButtonPressed);
        
        if (renameButtonPressed || (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_F2)))
            ImGui::OpenPopup("FileRenamePopup");

        if (deleteButtonPressed || (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Delete)))
            ImGui::OpenPopup("FileDeletePopup");

        
        ShowRenamePopup("FileRenamePopup", filename.c_str(), [this, path](std::string newName)
        {
            fs::rename(path, (path.parent_path() / newName).replace_extension(path.extension()));
            shouldReloadExplorer = true;
        });
        
        ShowDeletePopup("FileDeletePopup", filename.c_str(), [this, path]()
        {
            if (path.has_extension())
            {
                std::string resourcePath = path.string(); std::replace(resourcePath.begin(), resourcePath.end(), '\\', '/');
                if (resourceManager->Exists(resourcePath))
                    resourceManager->Delete(resourcePath);
            }
            fs::remove_all(path);
            shouldReloadExplorer = true;
        });

        // Show file or folder name.
        if (textSize.x < fileIconsSize)
        {
            ImGui::Indent((fileIconsSize - textSize.x) * 0.5f);
            ImGui::Text("%s", filename.c_str());
            ImGui::Unindent((fileIconsSize - textSize.x) * 0.5f);
        }
        else
        {
            size_t charCount = 0;
            float  curSize   = 0;
            for (; curSize < fileIconsSize && charCount < filename.size(); charCount++)
                curSize += ImGui::CalcCharSize(filename[charCount]).x;
            ImGui::Text("%s...", filename.substr(0, charCount-4).c_str());
        }
    }
    ImGui::EndChild();

    // Manual wrapping of file and folder icons.
    const float xCur = ImGui::GetItemRectMax().x;
    const float xNext = xCur + imGuiStyle->ItemSpacing.x + fileIconsSize+1; // Expected position if next button was on same line.
    if (xNext < xMax) ImGui::SameLine();
}

void Editor::ShowSoundUi(const std::string& id)
{
    const auto texture = miscIcons.at(miscIconPaths[13]);
    const auto textureSize = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) / std::max(texture->GetWidth(), texture->GetHeight()) * 50;
    
    SoundObject* soundObject = (SoundObject*)selectedGameObject;
    if (ImGui::CollapsingHeaderWithIcon(("Sound##" + id).c_str(), (ImTextureID)texture->GetId(), { textureSize.x, textureSize.y }, ImGuiTreeNodeFlags_AllowItemOverlap))
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
        ImGui::PushItemWidth(215);

        Sound* currentSound = soundObject->GetCurrentSound();
        {
            ImGui::AlignTextToFramePadding(); ImGui::Text("Sound"); ImGui::SameLine(); ImGui::Dummy({ 10, 0 }); ImGui::SameLine();
            if (ImGui::BeginCombo("##currSoundInput", (currentSound ? currentSound->GetName().c_str() : "none")))
            {
                {
                    const bool isSelected = !currentSound;
                    if (ImGui::Selectable("none", isSelected)) {
                        soundObject->SetCurrentSound(nullptr);
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                for (Sound* sound : soundObject->GetSounds())
                {
                    const bool isSelected = (sound->GetName() == (currentSound ? currentSound->GetName() : ""));
                    if (ImGui::Selectable(sound->GetName().c_str(), isSelected)) {
                        soundObject->SetCurrentSound(sound);
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        if (currentSound) {
            {
                Maths::Transform* transform = &selectedGameObject->transform;

                ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Miscellaneous settings");
                if (ImGui::BeginTable("PositionTable", 7, ImGuiTableFlags_SizingFixedFit))
                {
                    // Inputs for position.
                    Vector3 objectPos = transform->GetPosition();

                    ImGui::TableNextColumn();
                    ImGui::Text("Position");
                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);
                    if (ImGui::DragFloat("##PositionX", &objectPos.x, 0.1f)) {
                        transform->SetPosition(objectPos);
                    }

                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);

                    if (ImGui::DragFloat("##PositionY", &objectPos.y, 0.1f)) {
                        transform->SetPosition(objectPos);
                    }

                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);
                    if (ImGui::DragFloat("##PositionZ", &objectPos.z, 0.1f)) {
                        transform->SetPosition(objectPos);
                    }

                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("RotationTable", 7, ImGuiTableFlags_SizingFixedFit))
                {
                    // Inputs for rotation.
                    Vector3 objectRot = transform->GetEulerRot();
                    objectRot.x = radToDeg(objectRot.x); objectRot.y = radToDeg(objectRot.y); objectRot.z = radToDeg(objectRot.z);

                    ImGui::TableNextColumn();
                    ImGui::Text("Rotation");
                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[1]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);

                    if (ImGui::DragFloat("##RotationX", &objectRot.x, 0.1f)) {
                        objectRot.x = fmodf(objectRot.x, 360.f);
                        objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                        transform->SetEulerRot(objectRot);
                    }

                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[2]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);
                    if (ImGui::DragFloat("##RotationY", &objectRot.y, 0.1f)) {
                        objectRot.y = fmodf(objectRot.y, 360.f);
                        objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                        transform->SetEulerRot(objectRot);
                    }

                    ImGui::TableNextColumn();
                    ShowTextureUi(miscIcons.at(miscIconPaths[3]), 15);
                    ImGui::TableNextColumn(); ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3 });
                    ImGui::SetNextItemWidth(65.f);
                    if (ImGui::DragFloat("##RotationZ", &objectRot.z, 0.1f)) {
                        objectRot.z = fmodf(objectRot.z, 360.f);
                        objectRot.x = degToRad(objectRot.x); objectRot.y = degToRad(objectRot.y); objectRot.z = degToRad(objectRot.z);
                        transform->SetEulerRot(objectRot);
                    }

                    ImGui::EndTable();
                }
            }
            {
                float volume = currentSound->GetVolume();
                ImGui::Text("Volume"); ImGui::SameLine(); ImGui::Dummy({ 2, 0 }); ImGui::SameLine();
                if (ImGui::DragFloat("##Volume", &volume))
                {
                    currentSound->SetVolume(volume);
                }
            }

            {
                static bool isLooping = currentSound->IsLooping();
                ImGui::Text("Looping"); ImGui::SameLine();
                if (ImGui::Checkbox("##Looping", &isLooping))
                {
                    currentSound->SetLooping(isLooping);
                }
            }
            ImGui::NewLine();
            ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Spatialization settings");
            {
                bool isSpatialized = currentSound->IsSpatialized();
                ImGui::Text("Spatialized"); ImGui::SameLine(); ImGui::Dummy({ 2, 0 }); ImGui::SameLine();
                if (ImGui::Checkbox("##Spatialized", &isSpatialized))
                {
                    currentSound->SetSpatialization(isSpatialized);
                }

                float spatialGainMin = currentSound->GetSpatializationGainMin();
                float spatialGainMax = currentSound->GetSpatializationGainMax();
                ImGui::Text("Minimum gain"); ImGui::SameLine();
                if (ImGui::DragFloat("##MinGain", &spatialGainMin))
                {
                    currentSound->SetSpatializationMinGain(spatialGainMin);
                }
                ImGui::Text("Maximum gain"); ImGui::SameLine();
                if (ImGui::DragFloat("##MaxGain", &spatialGainMax))
                {
                    currentSound->SetSpatializationMaxGain(spatialGainMax);
                }
            }
            ImGui::NewLine();
            ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Cone settings");
            {
                float gain = currentSound->GetConeGain();
                ImGui::Text("Gain"); ImGui::SameLine(); ImGui::Dummy({ 55, 0 }); ImGui::SameLine();
                if (ImGui::DragFloat("##Gain", &gain))
                {
                    currentSound->SetAreaGain(gain);
                }
                float innerRadius = currentSound->GetConeInnerRadius();
                ImGui::Text("Inner Radius"); ImGui::SameLine();
                if (ImGui::DragFloat("##InnerRadius", &innerRadius))
                {
                    currentSound->SetAreaInnerRadius(innerRadius);
                }
                float outerRadius = currentSound->GetConeOuterRadius();
                ImGui::Text("Outer Radius"); ImGui::SameLine();
                if (ImGui::DragFloat("##OuterRadius", &outerRadius))
                {
                    currentSound->SetAreaOuterRadius(outerRadius);
                }
            }
            ImGui::NewLine();
            {
                float dopplerFactor = currentSound->GetDopplerFactor();
                ImGui::Text("Dopler Factor"); ImGui::SameLine();
                if (ImGui::DragFloat("##DoplerFactor", &dopplerFactor))
                {
                    currentSound->SetDopplerFactor(dopplerFactor);
                }
            }
            {
                float rollOff = currentSound->GetRollOff();
                ImGui::Text("Roll off"); ImGui::SameLine(); ImGui::Dummy({ 32, 0 }); ImGui::SameLine();
                if (ImGui::DragFloat("##RollOff", &rollOff))
                {
                    currentSound->SetRollOff(rollOff);
                }
            }

            ImGui::NewLine();
            ImGui::Bullet(); ImGui::SameLine(); ImGui::Text("Fade settings");
            {
                float fadeIn = currentSound->GetFadeIn();
                ImGui::Text("In"); ImGui::SameLine(); ImGui::Dummy({ 2, 0 }); ImGui::SameLine();
                if (ImGui::DragFloat("##FadeIn", &fadeIn))
                {
                    currentSound->SetFadeIn(fadeIn);
                }
            }
            {
                float fadeOut = currentSound->GetFadeOut();
                ImGui::Text("Out"); ImGui::SameLine();
                if (ImGui::DragFloat("##FadeOut", &fadeOut))
                {
                    currentSound->SetFadeOut(fadeOut);
                }
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5);
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 82.5);
            if (ImGui::Button("Play##Button", { 80 , 40 })) {
                if (currentSound->IsPlaying()) return;
                currentSound->Start();
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop##Button", { 80 , 40 })) {
                if (!currentSound->IsPlaying()) return;
                currentSound->Stop();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5);
       }
        ImGui::PopItemWidth();
    }
}

void Editor::SceneGraphWindow()
{
    if (ImGui::Begin("Scene Graph", NULL, ImGuiWindowFlags_NoCollapse))
    {
        if(ImGui::IsWindowHovered() && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)))
            selectedGameObject = nullptr;
        
        ShowObjectTreeNode(sceneManager->GetActiveScene()->GetSceneGraph()->GetRoot());
        
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) 
            ImGui::OpenPopup("SceneGraphPopup");
        
        ShowSceneGraphPopup();
    }
    ImGui::End();
}

void Editor::ObjectInspectorWindow()
{
    /*
    // This causes drag/dropping to be unavailable on a newly selected game object.
    {
        static GameObject* prevSelectedGameObject = selectedGameObject;
        if (prevSelectedGameObject != selectedGameObject && !ImGui::IsMouseDragging(0))
            ImGui::SetNextWindowFocus();
        prevSelectedGameObject = selectedGameObject;
    }
    */
     if (ImGui::Begin("Object Inspector", NULL, ImGuiWindowFlags_NoCollapse))
     {
        if (!selectedGameObject)
        {
            ImGui::Text("No selected game object.");
        }
        else
        {
            if(selectedGameObject->GetDestroyState()) return;
            
            GameObjectType selectedObjectType = selectedGameObject->GetType();
            if (ImGui::BeginTable("ObjectInfoTable", 2, ImGuiTableFlags_SizingFixedFit))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Show game object icon.
                size_t iconPathIdx = (size_t)selectedObjectType;
                if (selectedObjectType == GameObjectType::Light)
                {
                    const Light* light = ((LightObject*)selectedGameObject)->light;
                    if ((0 < light->outerCutoff && light->outerCutoff < PI*2) || (0 < light->innerCutoff && light->innerCutoff < PI*2))
                        iconPathIdx += 2;
                    else if (light->linear > 0 || light->quadratic > 0)
                        iconPathIdx += 1;
                }
                ShowTextureUi(objectIcons[objectIconPaths[iconPathIdx]], 100);

                // Show type, name, and ID.
                ImGui::TableNextColumn();
                ImGui::Dummy({ 0, (100-ImGui::GetTextLineHeightWithSpacing()*3.f)*0.5f });
                ImGui::Text("Type: %s",  GameObjectTypeToString(selectedObjectType).c_str());
                ImGui::Text("Name: %s",  selectedGameObject->name.c_str());
                ImGui::Text("ID: %zu",   selectedGameObject->GetId());

                ImGui::EndTable();
            }
            
            const std::string currentGameObjectId = std::to_string(selectedGameObject->GetId());
            
            ImGui::NewLine();
            if(selectedObjectType != GameObjectType::Canvas)
                ShowTransformUi(selectedGameObject->transform, "##Base" + currentGameObjectId);

                
            // Show info specific to game object type.
            switch (selectedObjectType)
            {
            case GameObjectType::Model:
                ShowMeshUi(((ModelObject*)selectedGameObject)->mesh, currentGameObjectId);
                break;
            case GameObjectType::Instanced:
                ShowInstancedUi((InstancedObject*)selectedGameObject, currentGameObjectId);
                break;
            case GameObjectType::Light:
                ShowLightUi(((LightObject*)selectedGameObject)->light, currentGameObjectId);
                break;
            case GameObjectType::Camera:
                ShowCameraUi(((CameraObject*)selectedGameObject)->camera, currentGameObjectId);
                break;
            case GameObjectType::Canvas:
                if (ImGui::Button("Edit canvas", {ImGui::GetWindowWidth() - imGuiStyle->WindowPadding.x * 2 - 0.5f, 40}))
                    selectedCanvas = (CanvasObject*)selectedGameObject;
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.5f);
                break;
            case GameObjectType::Cubemap:
                ShowCubemapUi(((CubemapObject*)selectedGameObject)->cubemap, currentGameObjectId);
                break;
            case GameObjectType::Sound:
                ShowSoundUi(currentGameObjectId);
                break;
            case GameObjectType::Empty:
            default:
                break;
            }
            
            ShowScriptUI(currentGameObjectId);
        }
    }
    ImGui::End();
}

void Editor::CanvasGraphWindow()
{
    if (ImGui::Begin("Canvas Graph"))
    {
        if(ImGui::IsWindowHovered() && (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)))
            selectedWidget = nullptr;
        
        if(selectedGameObject && selectedGameObject->GetType() == GameObjectType::Canvas)
        {
            ShowWidgetTreeNode(((CanvasObject*)selectedGameObject)->GetRoot());

            if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))  {
                ImGui::OpenPopup("CanvasGraphPopup");
            }
            ShowCanvasGraphPopup();
        }
    }
    ImGui::End();
}

void Editor::WidgetInspectorWindow()
{
    /*
    // This causes drag/dropping to be unavailable on a newly selected widget.
    {
        static Widget* prevSelectedWidget = selectedWidget;
        if (prevSelectedWidget != selectedWidget && !ImGui::IsMouseDragging(0))
            ImGui::SetNextWindowFocus();
        prevSelectedWidget = selectedWidget;
    }
    */
    if (ImGui::Begin("Widget Inspector", NULL, ImGuiWindowFlags_NoCollapse))
    {
        if (selectedCanvas)
        {
            if (!selectedWidget)
            {
                ImGui::Text("No selected Widget.");
            }
            else
            {
                WidgetType selectedWidgetType = selectedWidget->GetType();
                if (ImGui::BeginTable("WidgetInfoTable", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow(); ImGui::TableNextColumn();

                    // Show game object icon.
                    ShowTextureUi(widgetIcons[widgetIconPaths[(size_t)selectedWidgetType]], 100);

                    // Show type, name, and ID.
                    ImGui::TableNextColumn();
                    ImGui::Dummy({ 0, (100 - ImGui::GetTextLineHeightWithSpacing() * 3.f) * 0.5f });
                    ImGui::Text("Type: %s", WidgetTypeToString(selectedWidgetType).c_str());
                    ImGui::Text("Name: %s", selectedWidget->name.c_str());
                    ImGui::Text("ID: %zu", selectedWidget->GetId());

                    ImGui::EndTable();
                }

                const std::string currentSelectedWidgetId = std::to_string(selectedWidget->GetId());
                
                Show2DTransformUi(selectedWidget->position,selectedWidget->scale, selectedWidget->rotation, currentSelectedWidgetId);
                
                switch (selectedWidgetType)
                {
                case WidgetType::Text:
                    ShowTextWidgetUi((TextWidget*)selectedWidget, currentSelectedWidgetId);
                    break;
                case WidgetType::Button:
                    ShowButtonWidgetUi((ButtonWidget*)selectedWidget, currentSelectedWidgetId);
                    break;
                case WidgetType::Empty:
                default:
                    break;
                }
            }

        }
    }
    ImGui::End();
}

void Editor::FileExplorerWindow()
{
    if (ImGui::Begin("File Explorer"))
    {
        // Get all the parent paths of the current directory.
        static std::vector<fs::path> parentPaths;
        if (shouldReloadExplorer || parentPaths.empty() || parentPaths.back() != explorerPath)
        {
            shouldReloadExplorer = true;
            parentPaths.clear();
            fs::path curPath = explorerPath;
            while (!curPath.filename().string().empty())
            {
                parentPaths.insert(parentPaths.begin(), curPath);
                curPath = curPath.parent_path();
            }
        }
        
        // Show the path to the current directory.
        for (size_t i = 0; i < parentPaths.size(); i++)
        {
            if (ImGui::Button(parentPaths[i].filename().string().c_str())) {
                explorerPath = parentPaths[i];
            }
            // Enable dropping files and folders into parent folders.
            if (i < parentPaths.size()-1 && ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("File"))
                {
                    Assert(payload->DataSize == sizeof(void*), "File drag/drop payload of wrong size.");
                    const fs::path* droppedFile = *(fs::path**)payload->Data;
                    fs::rename(*droppedFile, parentPaths[i] / droppedFile->filename());
                    shouldReloadExplorer = true;
                }
                ImGui::EndDragDropTarget();
            }
            if (i < parentPaths.size() - 1) {
                ImGui::SameLine();
                ImGui::ArrowEx(parentPaths[i].filename().string().c_str(), ImGuiDir_Right);
                ImGui::SameLine();
            }
        }

        // Show a reload button.
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - imGuiStyle->FramePadding.x - ImGui::CalcTextSize("Reload").x - 100 - imGuiStyle->ItemSpacing.x - imGuiStyle->WindowPadding.x);
        if (ImGui::Button("Reload"))
            shouldReloadExplorer = true;

        // Show a slider to change the file icons size.
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x-100);
        ImGui::SetNextItemWidth(100);
        ImGui::SliderFloat("##fileIconsSizeSlider", &fileIconsSize, 50, 150, "%.0f");
        ImGui::Dummy({ 0, 5 });

        // Get all folders and files in the current directory whenever it changes.
        static std::vector<fs::path> folderPaths;
        static std::vector<fs::path> filePaths;
        if (shouldReloadExplorer)
        {
            folderPaths.clear();
            filePaths  .clear();
            for (const fs::directory_entry& entry : fs::directory_iterator(explorerPath))
            {
                if (!entry.path().has_extension()) folderPaths.push_back(entry.path());
                else                               filePaths  .push_back(entry.path());
            }
            shouldReloadExplorer = false;
        }
        
        // Show folders and files.
        const float xMax = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (const fs::path& folderPath : folderPaths) ShowFileUi(folderPath, xMax);
        for (const fs::path& filePath   : filePaths  ) ShowFileUi(filePath,   xMax);
    }
    ImGui::End();
}

void Editor::FileInspectorWindow()
{
    /*
    // This causes drag/dropping to be unavailable on a newly selected file.
    {
        static fs::path prevSelectedFile = selectedFile;
        if (prevSelectedFile != selectedFile && !ImGui::IsMouseDragging(0))
            ImGui::SetNextWindowFocus();
        prevSelectedFile = selectedFile;
    }
    */
    if (ImGui::Begin("File Inspector", NULL, ImGuiWindowFlags_NoCollapse))
    {
        if (selectedFile.empty())
        {
            ImGui::Text("No selected file.");
            ImGui::End();
            return;
        }

        std::string filePath   = selectedFile.string(); std::replace(filePath.begin(), filePath.end(), '\\', '/');
        const bool  fileLoaded = resourceManager->IsFileLoaded(filePath);
        if (ImGui::BeginTable("FileInfoTable", 2, ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            
            // Show file icon.
            const FileIcon fileIcon = GetFileIcon(selectedFile);
            ShowTextureUi(fileIcons[fileIconPaths[(size_t)fileIcon]], 100);

            // Show file type.
            ImGui::TableNextColumn();
            ImGui::Dummy({ 0, (100-ImGui::GetTextLineHeightWithSpacing()*3)*0.5f });
            std::string typeStr;
            switch (fileIcon)
            {
            case FileIcon::Code:     typeStr = "Script file";      break;
            case FileIcon::Sound:    typeStr = "Audio file";      break;
            case FileIcon::Image:    typeStr = "Image file";       break;
            case FileIcon::Material: typeStr = "Material file";    break;
            case FileIcon::Mesh:     typeStr = "Mesh file";        break;
            case FileIcon::Scene:    typeStr = "Scene file";       break;
            case FileIcon::Settings: typeStr = "Settings file";    break;
            case FileIcon::Shader:   typeStr = "Shader file";      break;
            case FileIcon::Font:     typeStr = "Font file";        break;
            case FileIcon::Folder:   typeStr = "Folder";           break;
            case FileIcon::Document: 
            default:                 typeStr = "Unsupported file"; break;
            }
            ImGui::Text("Type: %s", typeStr.c_str());

            // Show file state.
            ImGui::Text("State: %s", fileLoaded ? "Loaded" : "Unloaded");

            // Show file name.
            ImGui::Text("%s", filePath.c_str());

            ImGui::EndTable();
        }
        ImGui::NewLine();
        
        if (fileLoaded)
        {
            const std::vector<Resource*> fileResources = resourceManager->GetFileResources(filePath);
            const std::function showResourceUi = [this](Resource* resource)
            {
                switch (resource->GetType())
                {
                case ResourceType::Texture: {
                              Texture* texture          = (Texture*)resource;
                        const float    windowWidth      = ImGui::GetWindowWidth();
                        const float    normalizedHeight = (float)texture->GetHeight() / (float)texture->GetWidth();
                        ImGui::Image((ImTextureID)texture->GetId(), 
                                     { windowWidth, normalizedHeight * windowWidth }, 
                                     { 0, 1 }, { 1, 0 });
                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                        {
                            void** addressOfObject = new void*(texture);
                            ImGui::SetDragDropPayload("TexturePayload", addressOfObject, sizeof(void**));
                            ImGui::Text("Move %s", texture->GetName().c_str());
                            ImGui::EndDragDropSource();
                            delete addressOfObject;
                        }
                        break;
                }
                case ResourceType::Material:
                    ShowMaterialUi((Material*)resource, resource->GetName());
                    break;
                case ResourceType::Mesh:
                    ShowMeshUi((const Mesh*)resource, resource->GetName());
                    break;
                case ResourceType::VertexShader:
                    ImGui::TextColored({ 0.7f, 0.7f, 0.7f, 1 }, "%s", ((const VertexShader*)resource)->GetSourceCode().c_str());
                    break;
                case ResourceType::FragmentShader:
                    ImGui::TextColored({ 0.7f, 0.7f, 0.7f, 1 }, "%s", ((const FragmentShader*)resource)->GetSourceCode().c_str());
                    break;
                case ResourceType::Font:
                    // TODO: Show text using the font.
                    // renderer->DrawText("Lorem Ipsum", (const Font*)resource, {}, {1}, {1, 1, 1, 1}, 0);
                    break;
                default:
                    break;
                }
            };

            if (!fileResources.empty())
            {
                if (fileResources.size() == 1) {
                    showResourceUi(fileResources[0]);
                }
                else
                {
                    for (Resource* resource : fileResources)
                    {
                        if (ImGui::TreeNode(resource->GetName().c_str()))
                        {
                            showResourceUi(resource);
                            ImGui::TreePop();
                        }
                    }
                }
            }
        }
    }
    ImGui::End();
}

void Editor::PlayWindow() const
{
    ImGui::SetNextWindowPos({viewportPos.x + viewportSize.x*0.5f, viewportPos.y-1});
    if (ImGui::Begin("Start Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        Scene* scene = sceneManager->GetActiveScene();
        
        if (ImGui::ArrowButton("#play_button", ImGuiDir_Right))
        {
            if (scene->IsPaused())
                scene->Resume();
            else if (!scene->IsPlaying()) {
                scene->Start();
            }
        }
        ImGui::SameLine();
        if (ImGui::StraightLinesButton("#pause_button"))
        {
            scene->Pause();
        }
        ImGui::SameLine();
        if (ImGui::SquareButton("#stop_button"))
        {
            if (scene->IsPlaying() || scene->IsPaused())
                scene->Stop();
        }
    }
    ImGui::End();
}

void Editor::LogsWindow() const
{
    if (ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoCollapse))
    {
        static unsigned int prevLineCount = 0;
        unsigned int        lineCount     = 0;

        std::stringstream* logStream = Logger::GetLogs();
        std::string line;
        std::getline(*logStream, line);
        for (; std::getline(*logStream, line); lineCount++)
        {
            // Show infos in green.
            if (line.compare(0, 5, "INFO:") == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "INFO:");
                ImGui::PopStyleColor();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
                ImGui::TextWrapped("%s", line.substr(6, line.size()-6).c_str());
                ImGui::PopStyleColor();
            }

            // Show warnings in yellow.
            else if (line.compare(0, 8, "WARNING:") == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "WARNING:");
                ImGui::PopStyleColor();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
                ImGui::TextWrapped("%s", line.substr(9, line.size()-9).c_str());
                ImGui::PopStyleColor();
            }
            
            // Show errors in red.
            else if (line.compare(0, 6, "ERROR:") == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                ImGui::TextWrapped("ERROR:");
                ImGui::PopStyleColor();
                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
                ImGui::TextWrapped("%s", line.substr(7, line.size()-7).c_str());
                ImGui::PopStyleColor();
            }

            // Show the rest in white.
            else
            {
                ImGui::TextWrapped("%s", line.c_str());
            }
        }
        Logger::GetLogs()->clear();
        Logger::GetLogs()->seekg(0);

        // Scroll to the bottom when new logs arrive.
        if (lineCount != prevLineCount)
        {
            ImGui::SetScrollHereY(1.0);
            prevLineCount = lineCount;
        }
    }
    ImGui::End();
}

void Editor::GridDisplay() const
{
    /*
    const Scene*        scene        = sceneManager->GetActiveScene();
    const Camera*       camera       = scene->GetCameraManager()->GetEngineCamera();
    const LightManager* lightManager = scene->GetLightManager();
    
    ShaderProgram* shaderProgram = resourceManager->Get<ShaderProgram>("Resources/Shaders/Mesh/gridShader.prog");
    const Mesh*    mesh          = resourceManager->Get<Mesh>("mesh_Plane");

    shaderProgram->SetScript(new GridMeshShaderScript());
    mesh->subMeshes[0]->SetShaderProgram(shaderProgram);

    renderer->BindRenderTexture(objectRenderTexture);
    renderer->DrawSubMesh(mesh->subMeshes[0], Mat4::FromScale({100}), camera, lightManager);
    renderer->UnbindRenderTexture();
    */
}

void Editor::ViewportWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    const bool windowOpen = ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();
    
    if (windowOpen)
    {
        DrawObjectOutline();
        DrawObjectGizmo();
        
        // Draw the scene or viewport texture in the viewport.
        const Vector2 textureSize = window->GetSize();
        const Vector2 ratio  = viewportSize / textureSize;
        const Vector2 offset = (Vector2(1) - ratio) * 0.5f;
        const ImTextureID textureId = selectedGameObject ? (ImTextureID)viewportRenderTexture->GetPipelineData().ID : (ImTextureID)engineRenderTexture->GetPipelineData().ID;
        ImGui::Image(textureId, { viewportSize.x, viewportSize.y }, { offset.x, ratio.y + offset.y }, { ratio.x + offset.x, offset.y });

        // Store viewport position and size for later use.
        const ImVec2 pos = ImGui::GetItemRectMin();
        viewportPos      = Vector2(pos.x, pos.y);
        viewportSize     = Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        if (ImGui::IsWindowHovered())
        {
            window->SetViewportPos (viewportPos);
            window->SetViewportSize(viewportSize);
        }

        ObjectPicking();
        GizmoBehavior();
        GridDisplay();
        
        if (selectedCanvas && ImGui::IsWindowFocused())
            selectedCanvas = nullptr;
    }
    ImGui::End();
}

void Editor::SceneViewWindow()
{
    static SceneState prevSceneState = sceneManager->GetActiveScene()->GetState();
    if (prevSceneState != sceneManager->GetActiveScene()->GetState() && prevSceneState != SceneState::Playing)
        ImGui::SetNextWindowFocus();
    prevSceneState = sceneManager->GetActiveScene()->GetState();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    const bool windowOpen = ImGui::Begin("Scene View", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();
    
    if (windowOpen)
    {
        // Draw the scene texture in the scene view window.
        const Scene*  scene = sceneManager->GetActiveScene();
        const Vector2 textureSize = window->GetSize();
        const Vector2 ratio  = sceneViewSize / textureSize;
        const Vector2 offset = (Vector2(1) - ratio) * 0.5f;
        const ImTextureID textureId = (ImTextureID)scene->GetRenderTexture()->GetPipelineData().ID;
        ImGui::Image(textureId, { sceneViewSize.x, sceneViewSize.y }, { offset.x, ratio.y + offset.y }, { ratio.x + offset.x, offset.y });

        // Store viewport position and size for later use.
        const ImVec2 pos = ImGui::GetItemRectMin();
        sceneViewPos  = Vector2(pos.x, pos.y);
        sceneViewSize = Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        if (ImGui::IsWindowHovered())
        {
            window->SetViewportPos (sceneViewPos);
            window->SetViewportSize(sceneViewSize);
            ImGui::SetNextFrameWantCaptureMouse   (false);
            ImGui::SetNextFrameWantCaptureKeyboard(false);
        }
        
        if (selectedCanvas && ImGui::IsWindowFocused())
            selectedCanvas = nullptr;
    }
    ImGui::End();
}

void Editor::CanvasPanelWindow()
{
    {
        static CanvasObject* prevSelectedCanvas = selectedCanvas;
        if (prevSelectedCanvas != selectedCanvas)
            ImGui::SetNextWindowFocus();
        prevSelectedCanvas = selectedCanvas;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    const bool windowOpen = ImGui::Begin("Canvas Panel", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();

    if (windowOpen)
    {
        // Render on the canvas panel render texture.
        canvasRenderTexture->BeginRender();
        static Texture* defaultTexture = resourceManager->Get<Texture>("Resources/Textures/UI/canvasPanel.jpg");
        renderer->DrawTexture2D(defaultTexture, Vector2(2), Vector2(0), Vector2(1), 0);
        if (selectedCanvas) selectedCanvas->Draw(renderer);
        canvasRenderTexture->EndRender();

        // Draw the canvas panel render texture in ImGui.
        const Vector2 textureSize = window->GetSize();
        const Vector2 ratio = canvasPanelSize / textureSize;
        const Vector2 offset = (Vector2(1) - ratio) * 0.5f;
        ImGui::Image((ImTextureID)canvasRenderTexture->GetPipelineData().ID, { canvasPanelSize.x, canvasPanelSize.y }, { offset.x, ratio.y + offset.y }, { ratio.x + offset.x, offset.y });
        
        // Store viewport position and size for later use.
        const ImVec2 pos = ImGui::GetItemRectMin();
        canvasPanelPos   = Vector2(pos.x, pos.y);
        canvasPanelSize  = Vector2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        if (ImGui::IsWindowHovered())
        {
            window->SetViewportPos (canvasPanelPos);
            window->SetViewportSize(canvasPanelSize);
        }
          
        WidgetPicking();
    }
    ImGui::End();
}

void Editor::DrawObjectOutline() const
{
    if (!selectedGameObject) return;
    
    // Draw the selected game object on the object texture.
    objectRenderTexture->BeginRender();
    const Mat4          modelMat     = selectedGameObject->transform.GetWorldMat();
    const Scene*        scene        = sceneManager->GetActiveScene();
    const Camera*       camera       = scene->GetCameraManager()->GetEngineCamera();
    const LightManager* lightManager = scene->GetLightManager();
    // TODO: Find a cleaner way to draw elements in a plain color.
    switch (selectedGameObject->GetType())
    {
    case GameObjectType::Model:
    {
        const ModelObject* modelObject = (const ModelObject*)selectedGameObject;
        for (const SubMesh* subMesh : modelObject->mesh->subMeshes)
        {
            const ShaderProgram* shaderProgram = subMesh->GetShaderProgram(); if (!shaderProgram) continue;
            DefaultMeshShaderScript* script = dynamic_cast<DefaultMeshShaderScript*>(shaderProgram->GetShaderScript());
            script->usePlainColor = true;
            script->plainColor = { 1, 1, 1, 1 };
            renderer->DrawSubMesh(subMesh, modelMat, camera, lightManager, 0);
            script->usePlainColor = false;
        }
        break;
    }
    case GameObjectType::Instanced:
    {
        const InstancedObject* instancedObject = (const InstancedObject*)selectedGameObject;
        for (const SubMesh* subMesh : instancedObject->mesh->subMeshes)
        {
            const ShaderProgram* shaderProgram = subMesh->GetInstancedShaderProgram(); if (!shaderProgram) continue;
            DefaultMeshShaderScript* script = dynamic_cast<DefaultMeshShaderScript*>(shaderProgram->GetShaderScript());
            script->usePlainColor = true;
            script->plainColor = { 1, 1, 1, 1 };
            renderer->DrawInstanced(subMesh, (int)instancedObject->instanceCount, modelMat, camera, lightManager, 0);
            script->usePlainColor = false;
        }
        break;
    }
    default:
        break;
    }
    objectRenderTexture->EndRender();

    // Draw the scene and object textures on the viewport texture. 
    viewportRenderTexture->BeginRender();
    renderer->DrawScreenTexture(engineRenderTexture);
    static ShaderProgram* edgeShader = nullptr;
    if (!edgeShader) edgeShader = resourceManager->Get<ShaderProgram>("Resources/Shaders/Screen/edgeDetection.prog");
    if ( edgeShader) renderer->DrawScreenTexture(objectRenderTexture, edgeShader);
    renderer->ClearBindedRenderTexture({}, true);
    viewportRenderTexture->EndRender();
}

void Editor::DrawObjectGizmo() const
{
    if (!selectedGameObject) return;

    // Get the gizmo mesh.
    static Mesh*     gizmos   [3] = { nullptr, nullptr, nullptr };
    static Material* materials[3] = { nullptr, nullptr, nullptr };
    if (!gizmos[0] || !gizmos[1] || !gizmos[2]) {
        gizmos[0] = resourceManager->Get<Mesh>("mesh_gizmoTranslation");
        gizmos[1] = resourceManager->Get<Mesh>("mesh_gizmoRotation");
        gizmos[2] = resourceManager->Get<Mesh>("mesh_gizmoScaling");
        for (int i = 0; i < 3; i++)
            materials[i] = gizmos[0]->subMeshes[i]->GetMaterial();
        return;
    }

    // Highlight the selected axis.
    static Vector3 prevGizmoAxis = gizmoAxis;
    if (prevGizmoAxis != gizmoAxis)
    {
        if (gizmoAxis != Vector3())
        {
            if      (gizmoAxis == Vector3(1, 0, 0)) materials[0]->emission = { 0.917647f, 0.019608f, 0         };
            else if (gizmoAxis == Vector3(0, 1, 0)) materials[1]->emission = { 0,         0.529411f, 0.211764f };
            else if (gizmoAxis == Vector3(0, 0, 1)) materials[2]->emission = { 0,         0.105882f, 0.925491f };
        }
        else
        {
            materials[0]->emission = { 0.768151f, 0.05448f,  0.03434f  };
            materials[1]->emission = { 0,         0.366253f, 0.14996f  };
            materials[2]->emission = { 0.052861f, 0.141263f, 0.752943f };
        }
    }
    prevGizmoAxis = gizmoAxis;

    // Get necessary classes and data to draw.
    const Scene*        scene        = sceneManager->GetActiveScene();
    const Camera*       camera       = scene->GetCameraManager()->GetEngineCamera();
    const LightManager* lightManager = scene->GetLightManager();
    const Mat4 modelMat = Mat4::FromScale(Vector3(selectedGameObject->transform.GetWorldPosition(), camera->transform->GetWorldPosition()).GetLength() * 0.2f)
                        * Mat4::FromTranslation(selectedGameObject->transform.GetWorldPosition());
    
    // Draw the gizmo mesh.
    renderer->BindRenderTexture(viewportRenderTexture);
    for (size_t i = 0; i < gizmos[(int)gizmoType]->subMeshes.size(); i++)
        renderer->DrawSubMesh(gizmos[(int)gizmoType]->subMeshes[i], modelMat, camera, lightManager, i+1);
    renderer->UnbindRenderTexture();
}

void Editor::GizmoBehavior()
{
    if (!selectedGameObject) return;
    
    const WindowInputs inputs = window->GetInputs();
    if (gizmoAxis == Vector3() || !inputs.mouseLeftClick)
    {
        if      (ImGui::IsKeyDown(ImGuiKey_LeftShift)) gizmoType = GizmoType::Rotation;
        else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl )) gizmoType = GizmoType::Scaling;
        else                                           gizmoType = GizmoType::Translation;
    }
    else
    {
        const Camera* camera       = sceneManager->GetActiveScene()->GetCameraManager()->GetEngineCamera();
        const Mat4    viewProjMat  = camera->GetViewMat() * camera->GetProjectionMat();
        const Mat3    parentInvRot = selectedGameObject->transform.GetParentMat().ToMat3().Inv3();
        const Vector3 gizmoPos     = selectedGameObject->transform.GetWorldPosition();
        const Vector2 gizmoPos2D   = ( gizmoPos              * viewProjMat).ToVector2();
        const Vector2 gizmoTip2D   = ((gizmoPos + gizmoAxis) * viewProjMat).ToVector2();
        const Vector2 gizmoAxis2D  = Vector2({ gizmoPos2D.x, gizmoPos2D.y }, { gizmoTip2D.x, gizmoTip2D.y }).GetNormalized();

        // Apply translation and scaling.
        if (gizmoType != GizmoType::Rotation)
        {
            const Vector2 mouseDelta = { inputs.mouseDelta.x, -inputs.mouseDelta.y };
            if (mouseDelta == Vector2()) return;
            
            const float amount = gizmoAxis2D.Dot(mouseDelta) * 0.01f;
            if (gizmoType == GizmoType::Translation) selectedGameObject->transform.Move ((gizmoAxis * parentInvRot).GetNormalized() * amount);
            else                                     selectedGameObject->transform.Scale((gizmoAxis * parentInvRot).GetNormalized() * amount);
        }

        // Apply rotation.
        else
        {
            const Vector2 mouseDelta = { inputs.mouseDelta.x, -inputs.mouseDelta.y };
            if (mouseDelta == Vector2()) return;
            
            const Vector2 screenGizmo   = (gizmoPos2D + 1) * 0.5f * viewportSize;
            const Vector2 mousePos      = Vector2(inputs.mousePos.x, viewportSize.y-inputs.mousePos.y) - viewportPos;
            const Vector2 gizmoToMouse1 = { screenGizmo, mousePos };
            const Vector2 gizmoToMouse2 = { screenGizmo, mousePos - mouseDelta };
            const int     clockwise     = signof(gizmoToMouse1.GetAngleSigned(gizmoToMouse2));

            const Vector3 objectToCam = Vector3(selectedGameObject->transform.GetWorldPosition(), camera->transform->GetWorldPosition()) * gizmoAxis;
            const int     reversed    = objectToCam.x >= 0 && objectToCam.y >= 0 && objectToCam.z >= 0 ? 1 : -1;
            
            const Vector3    worldDelta = (camera->transform->GetWorldMat() * Vector4(mouseDelta.x, mouseDelta.y, 0, 0)).ToVector3();
            const Vector3    cross      = gizmoAxis.Cross(worldDelta);
            const float      amount     = cross.GetLength() * 0.01f;
            const Vector3    eulerRot   = (gizmoAxis * parentInvRot).GetNormalized() * amount * clockwise * reversed;
            selectedGameObject->transform.Rotate(Quaternion::FromEuler(eulerRot));
        }
    }
}

void Editor::ObjectPicking()
{
    // Pick objects from viewport.
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            const Vector2 mouseRelative = window->GetInputs().mousePos - viewportPos;

            // Click on gizmo.
            const size_t gizmoId = renderer->PickObjectFromViewport(mouseRelative, viewportRenderTexture, viewportSize);
            switch(gizmoId)
            {
                case 1:  gizmoAxis = Vector3(1, 0, 0); break;
                case 2:  gizmoAxis = Vector3(0, 1, 0); break;
                case 3:  gizmoAxis = Vector3(0, 0, 1); break;
                default: gizmoAxis = Vector3(0, 0, 0); break;
            }

            // Pick game objects.
            if (gizmoId <= 0)
            {
                const size_t objectId = renderer->PickObjectFromViewport(mouseRelative, engineRenderTexture, viewportSize);
                if (objectId > 0) selectedGameObject = sceneManager->GetActiveScene()->GetSceneGraph()->Find(objectId);
                else              selectedGameObject = nullptr;
            }
            
            ImGui::SetNextFrameWantCaptureMouse(true);
        }
        else
        {
            ImGui::SetNextFrameWantCaptureMouse   (false);
            ImGui::SetNextFrameWantCaptureKeyboard(false);
        }
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && gizmoAxis != Vector3())
    {
        gizmoAxis = Vector3();
    }
}

void Editor::WidgetPicking()
{
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            const Vector2 mouseRelative = window->GetInputs().mousePos - viewportPos;

            const size_t  objectId = 256 - renderer->PickObjectFromViewport(mouseRelative, sceneManager->GetActiveScene()->GetRenderTexture(), viewportSize);
            if (!objectId) selectedWidget = nullptr;
            else           selectedWidget = selectedCanvas->FindWidget(objectId);
        }
        ImGui::SetNextFrameWantCaptureMouse   (false);
        ImGui::SetNextFrameWantCaptureKeyboard(false);
    }
}

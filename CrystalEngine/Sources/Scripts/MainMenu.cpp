#include"Scripts/MainMenu.h"

#include "Core/Engine.h"
#include "Core/SoundManager.h"

#include "Interface/Widget.h"

#include "Resources/ResourceManager.h"

#include "Scenes/GameObject.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"

#include "Refureku/Generated/MainMenu.rfks.h"

using namespace Scripts;
using namespace Scenes;
using namespace Interface;
using namespace Resources;
using namespace Core;

MainMenu::MainMenu()
{
    archId = staticGetArchetype().getId();
}

void MainMenu::Start()
{
    static SceneGraph* sceneGraph = engine->GetSceneManager()->GetActiveScene()->GetSceneGraph();
    static SceneManager* sceneManager = engine->GetSceneManager();

    GameObject* mainTheme = engine->GetSceneManager()->GetActiveScene()->GetSceneGraph()->Find("mainTheme");

    mainSound = engine->GetResourceManager()->Get<Sound>("Resources/Sounds/Dark-Forest.mp3");

    mainSound->SetLooping(true);

    ((SoundObject*)mainTheme)->AddSound("Resources/Sounds/Dark-Forest.mp3");
    ((SoundObject*)mainTheme)->SetCurrentSound(mainSound);
    ((SoundObject*)mainTheme)->PlayCurrentSound();

    CanvasObject* MainMenu = (CanvasObject*)sceneGraph->Find("Canvas");
    ButtonWidget* playButton = (ButtonWidget*)MainMenu->FindWidget("PlayButton");
    ButtonWidget* quitButton = (ButtonWidget*)MainMenu->FindWidget("QuitButton");

    playButton->AddCallback([] { sceneManager->SetActiveScene("Resources/Scenes/GameScene.json", true); });

    quitButton->AddCallback([this] {engine->Quit(); });

}

void MainMenu::Update()
{


}
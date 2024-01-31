#pragma once

#include <../Externals/Includes/pybind/cast.h>

#include "Maths/Maths.h"
#include "Debug/Logger.h"

#include "Core/Engine.h"
#include "Core/EventDispatcher.h"
#include "Core/SoundManager.h"
#include "Core/ThreadManager.h"
#include "Core/TimeManager.h"
#include "Core/Window.h"

#include "Resources/ResourceManager.h"
#include "Resources/AssimpParser.h"
#include "Resources/WavefrontParser.h"
#include "Resources/Texture.h"
#include "Resources/RenderTexture.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Resources/Shader.h"
#include "Resources/ShaderProgram.h"
#include "Resources/Cubemap.h"
#include "Resources/Font.h"

#include "Animation/Animator.h"
#include "Animation/Animator.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"

#include "Render/Renderer.h"
#include "Render/PostProcessor.h"
#include "Render/Light.h"
#include "Render/LightManager.h"
#include "Render/Camera.h"
#include "Render/CameraManager.h"

#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/GameObject.h"

#include "Interface/Widget.h"

#include "Scripts/ObjectScript.h"
#include "Scripts/ShaderScript.h"
#include "Scripts/PythonScript.h"
#include "Scripts/LuaScript.h"

PYBIND11_MAKE_OPAQUE(Maths::RGB);
PYBIND11_MAKE_OPAQUE(Maths::RGBA);
PYBIND11_MAKE_OPAQUE(Maths::HSV);
PYBIND11_MAKE_OPAQUE(Maths::HSVA);
PYBIND11_MAKE_OPAQUE(Maths::Vector2);
PYBIND11_MAKE_OPAQUE(Maths::Vector3);
PYBIND11_MAKE_OPAQUE(Maths::Vector4);
PYBIND11_MAKE_OPAQUE(Maths::Mat2);
PYBIND11_MAKE_OPAQUE(Maths::Mat3);
PYBIND11_MAKE_OPAQUE(Maths::Mat4);
PYBIND11_MAKE_OPAQUE(Maths::AngleAxis);
PYBIND11_MAKE_OPAQUE(Maths::Transform);

PYBIND11_MAKE_OPAQUE(Debug::Logger);

PYBIND11_MAKE_OPAQUE(Core::Engine);
PYBIND11_MAKE_OPAQUE(Core::EventDispatcher);
PYBIND11_MAKE_OPAQUE(Core::SoundManager);
PYBIND11_MAKE_OPAQUE(Core::ThreadManager);
PYBIND11_MAKE_OPAQUE(Core::TimeManager);
PYBIND11_MAKE_OPAQUE(Core::Window);

PYBIND11_MAKE_OPAQUE(Resources::ResourceManager);
PYBIND11_MAKE_OPAQUE(Resources::AssimpParser);
PYBIND11_MAKE_OPAQUE(Resources::WavefrontParser);
// PYBIND11_MAKE_OPAQUE(Resources::Resource);
PYBIND11_MAKE_OPAQUE(Resources::Texture);
PYBIND11_MAKE_OPAQUE(Resources::RenderTexture);
PYBIND11_MAKE_OPAQUE(Resources::Material);
PYBIND11_MAKE_OPAQUE(Resources::Mesh);
// PYBIND11_MAKE_OPAQUE(Resources::SubMesh);
PYBIND11_MAKE_OPAQUE(Resources::StaticSubMesh);
PYBIND11_MAKE_OPAQUE(Resources::AnimatedSubMesh);
PYBIND11_MAKE_OPAQUE(Resources::VertexShader);
PYBIND11_MAKE_OPAQUE(Resources::FragmentShader);
PYBIND11_MAKE_OPAQUE(Resources::ShaderProgram);
PYBIND11_MAKE_OPAQUE(Resources::Cubemap);
PYBIND11_MAKE_OPAQUE(Resources::Font);

PYBIND11_MAKE_OPAQUE(Anim::Animator);
PYBIND11_MAKE_OPAQUE(Anim::Animation);
PYBIND11_MAKE_OPAQUE(Anim::Skeleton);
PYBIND11_MAKE_OPAQUE(Anim::Bone);

PYBIND11_MAKE_OPAQUE(Render::Renderer);
PYBIND11_MAKE_OPAQUE(Render::PostProcessor);
PYBIND11_MAKE_OPAQUE(Render::CameraManager);
PYBIND11_MAKE_OPAQUE(Render::Camera);
PYBIND11_MAKE_OPAQUE(Render::LightManager)
PYBIND11_MAKE_OPAQUE(Render::Light);

PYBIND11_MAKE_OPAQUE(Scenes::SceneManager);
PYBIND11_MAKE_OPAQUE(Scenes::Scene);
PYBIND11_MAKE_OPAQUE(Scenes::SceneGraph);
PYBIND11_MAKE_OPAQUE(Scenes::GameObject);
PYBIND11_MAKE_OPAQUE(Scenes::ModelObject);
PYBIND11_MAKE_OPAQUE(Scenes::AnimatedObject);
PYBIND11_MAKE_OPAQUE(Scenes::InstancedObject);
PYBIND11_MAKE_OPAQUE(Scenes::CameraObject);
PYBIND11_MAKE_OPAQUE(Scenes::LightObject);
PYBIND11_MAKE_OPAQUE(Scenes::SoundObject);
PYBIND11_MAKE_OPAQUE(Scenes::CubemapObject);
PYBIND11_MAKE_OPAQUE(Scenes::CanvasObject);

PYBIND11_MAKE_OPAQUE(Interface::Widget);
PYBIND11_MAKE_OPAQUE(Interface::ButtonWidget);
PYBIND11_MAKE_OPAQUE(Interface::TextWidget);

PYBIND11_MAKE_OPAQUE(Scripts::ObjectScript);
PYBIND11_MAKE_OPAQUE(Scripts::ShaderScript);
PYBIND11_MAKE_OPAQUE(Scripts::ScreenShaderScript);
PYBIND11_MAKE_OPAQUE(Scripts::MeshShaderScript);
PYBIND11_MAKE_OPAQUE(Scripts::PythonScript);
PYBIND11_MAKE_OPAQUE(Scripts::LuaScript);

PYBIND11_MAKE_OPAQUE(Utils::IdGenerator);

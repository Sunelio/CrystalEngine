#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/EventDispatcher.h"
#include "Render/PostProcessor.h"
#include "Render/Renderer.h"
#include "Debug/Logger.h"
#include "Resources/Shader.h"
#include "Resources/RenderTexture.h"
#include "Resources/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Scripts/ShaderScript.h"
#include "Scripts/DefaultScripts.h"
#include "LightJson/JsonElement.h"
#include <cstdarg>
using namespace Maths;
using namespace Resources;
using namespace Render;
using namespace Core;

PostProcessor::PostProcessor()
{
	const Engine* engine = Engine::Get();
	const Window* window = engine->GetWindow();
	renderer = engine->GetRenderer();
	
	// Create the render textures.
	pingPongTexture = new RenderTexture("PingPongTexture");
	pingPongTexture->SetSize(window->GetWidth(), window->GetHeight());
	pingPongTexture->Load();
	pingPongTexture->SendToPipeline();

	// Setup the screen resize event callback.
	Engine::Get()->GetEventDispatcher()->AddListener<ScreenResizeEvent>([this](Event* event)
	{
		const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
		if (pingPongTexture) pingPongTexture->SetSize(resizeEvent->width, resizeEvent->height);
	});
}

PostProcessor::~PostProcessor()
{
	delete pingPongTexture;
}

void PostProcessor::BeginRender(const RenderTexture* renderTexture) const
{
	if (renderTexture)
		renderTexture->BeginRender();
	else
		DebugLogWarning("Unable to render using post processor: it is not initialized.");
}

void PostProcessor::EndRender(RenderTexture* renderTexture) const
{
	// Do a render pass for each post-processing shader, using ping pong texturing.
	if (!renderTexture || !pingPongTexture) return;
	const RenderTexture* renderTextures[2] = { renderTexture, pingPongTexture };
	const size_t renderPasses = shaders.size() + shaders.size() % 2;
	for (size_t i = 0; i < renderPasses; i++)
	{
		const RenderTexture* source = renderTextures[i%2];
		const RenderTexture* dest   = renderTextures[1-i%2];
		
		dest->BeginRender();
		renderer->DrawScreenTexture(source, i < shaders.size() ? shaders[i] : nullptr);
		dest->EndRender();
	}
	renderer->UnbindRenderTexture();
	renderer->ClearBindedRenderTexture({ 0, 0, 0, 1 });
}

void PostProcessor::AddShader(FragmentShader* shader, Scripts::ScreenShaderScript* script)
{
	const Engine* engine = Engine::Get(); if (!engine) return;
	ResourceManager* resourceManager = engine->GetResourceManager(); if (!resourceManager) return;

	// Get the shader program name.
	std::string name = shader->GetName();
	if (name.substr(name.size()-5, 5) != ".frag") return;
	name = name.substr(0, name.size()-5) + ".prog";

	// Create the post-processing shader program.
	ShaderProgram* shaderProgram;
	if (!resourceManager->Exists(name)) {
		shaderProgram = resourceManager->Create<ShaderProgram>(name);
		shaderProgram->AttachShaders(2, resourceManager->Get<VertexShader>("Resources/Shaders/Screen/screenShader.vert"), shader);
		if (script) shaderProgram->SetScript(script);
		else        shaderProgram->SetScript(new Scripts::DefaultScreenShaderScript);
		shaderProgram->Load();
		shaderProgram->SendToPipeline();
	}
	else {
		shaderProgram = resourceManager->Get<ShaderProgram>(name);
	}
	AddShaderProgram(shaderProgram);
}

void PostProcessor::AddShaderProgram(ShaderProgram* shaderProgram)
{
	if (std::count(shaders.begin(), shaders.end(), shaderProgram) <= 0)
		shaders.push_back(shaderProgram);
}

void PostProcessor::AddShaderPrograms(size_t count, ...)
{
	va_list args;
	va_start(args, count);
	for (size_t i = 0; i < count; i++)
		AddShaderProgram(va_arg(args, ShaderProgram*));
	va_end(args);
}

void PostProcessor::RemoveShaderProgram(const ShaderProgram* shaderProgram)
{
	const auto position = std::find(shaders.begin(), shaders.end(), shaderProgram);
	if (position != shaders.end())
		shaders.erase(position);
}

LightJson::JsonElement* PostProcessor::Serialize() const
{
	LightJson::JsonUtils::JsonArray array{}; array.reserve(shaders.size());
	for (const ShaderProgram* shaderProgram : shaders)
	{
		LightJson::JsonUtils::JsonArray shaderData{};
		for (const Resource* shader : shaderProgram->GetAttachedShaders())
			shaderData.push_back(new LightJson::StringElement(shader->GetName()));
		shaderData.push_back(new LightJson::StringElement(shaderProgram->GetName()));
		array.push_back(new LightJson::ArrayElement(shaderData));
	}
	return new LightJson::ArrayElement(array);
}

void PostProcessor::Deserialize(const LightJson::JsonElement* jsonElement)
{
	static ResourceManager* resourceManager = Engine::Get()->GetResourceManager();
	std::vector<Resource*> attachedShaders{}; attachedShaders.reserve(2);
	
	shaders.clear();
	const LightJson::JsonUtils::JsonArray programArray = jsonElement->getAsArray(); shaders.reserve(programArray.size());
	for (const LightJson::JsonElement* programElement : programArray)
	{
		const LightJson::JsonUtils::JsonArray filenameArray = programElement->getAsArray();
		for (const LightJson::JsonElement* filenameElement : filenameArray)
		{
			const std::string filename  = filenameElement->getAsString();
			const std::string extension = std::filesystem::path(filename).extension().string();
			if (extension == ".vert")
			{
				resourceManager->LoadFile(filename, false);
				attachedShaders.push_back(resourceManager->Get<VertexShader>(filename));
			}
			if (extension == ".frag")
			{
				resourceManager->LoadFile(filename, false);
				attachedShaders.push_back(resourceManager->Get<FragmentShader>(filename));
			}
			if (extension == ".prog")
			{
				ShaderProgram* shaderProgram = resourceManager->Create<ShaderProgram>(filename);
				if (!shaderProgram->WasSentToPipeline())
				{
					for (Resource* shader : attachedShaders)
						shaderProgram->AttachShader(shader);
					shaderProgram->SetScript(new Scripts::DefaultScreenShaderScript);
					shaderProgram->Load();
					shaderProgram->SendToPipeline();
				}
				AddShaderProgram(shaderProgram);
				break;
			}
		}
		attachedShaders.clear();
	}
}

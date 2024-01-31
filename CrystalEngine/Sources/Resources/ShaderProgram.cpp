#include "Resources/ShaderProgram.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Debug/Logger.h"
#include "Scripts/ShaderScript.h"
#include "Scripts/DefaultScripts.h"
#include <cstdarg>
using namespace Maths;
using namespace Resources;

ShaderProgram::ShaderProgram(const std::string& _name)
{
	name = _name;
	type = ResourceType::ShaderProgram;
}

ShaderProgram::~ShaderProgram()
{
	delete script;
	if (WasSentToPipeline())
		Core::Engine::Get()->GetRenderer()->DeleteResource<ShaderProgram>(this);
}

void ShaderProgram::AttachShader(Resource* shader)
{
	if (shader->GetType() == ResourceType::VertexShader ||
		shader->GetType() == ResourceType::FragmentShader)
	{
		if (std::find(attachedShaders.begin(), attachedShaders.end(), shader) == attachedShaders.end())
			attachedShaders.push_back(shader);
	}
	else
	{
		DebugLogError("Tried to attach a resource of type " + Resources::ResourceTypeToString(shader->GetType()) + " to a shader program.");
	}
}

void ShaderProgram::AttachShaders(const size_t count, ...)
{
	va_list args;
	va_start(args, count);
	for (size_t i = 0; i < count; i++)
		AttachShader(va_arg(args, Resource*));
	va_end(args);
}

void ShaderProgram::SetScript(Scripts::ShaderScript* _script)
{
	delete script;
	script = _script;
	script->shader = this;
}

void ShaderProgram::Load()
{
	if (attachedShaders.empty() || IsLoaded())
		return;
	
	for (Resource* shader : attachedShaders)
		if (!shader->IsLoaded())
			shader->Load();
	loaded = true;
}

void ShaderProgram::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;
	
	// Check that all attached shaders were sent to the pipeline.
	for (Resource* shader : attachedShaders) {
		if (!shader->WasSentToPipeline()) {
			shader->SendToPipeline();
		}
	}

	// Add the default mesh shader script if none is set.
	if (!script) {
		SetScript(new Scripts::DefaultMeshShaderScript);
	}
	
	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<ShaderProgram>(this);
	sentToPipeline = true;
}
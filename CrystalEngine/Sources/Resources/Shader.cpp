#include "Resources/Shader.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Debug/Logger.h"
#include <fstream>
#include <sstream>
using namespace Resources;

static std::string LoadShader(const std::string& filename)
{
	// Load shader source.
	std::string fileStr;
	{
		std::stringstream fileContents;
		std::fstream f(filename, std::ios_base::in | std::ios_base::app);
		if (!f.is_open())
		{
			DebugLogError("Unable to open shader file: " + filename);
			return "";
		}
		fileContents << f.rdbuf();
		fileStr = fileContents.str();
		f.close();
	}
	return fileStr;
}


VertexShader::VertexShader(const std::string& _name)
{
	name = _name;
	type = ResourceType::VertexShader;
}

VertexShader::~VertexShader()
{
	if (WasSentToPipeline())
		Core::Engine::Get()->GetRenderer()->DeleteResource<VertexShader>(this);
}

void VertexShader::Load()
{
	if (IsLoaded())
		return;
	
	sourceCode = LoadShader(name);
	loaded = true;
}

void VertexShader::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;
	
	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<VertexShader>(this);
	sentToPipeline = true;
}



FragmentShader::FragmentShader(const std::string& _name)
{
	name = _name;
	type = ResourceType::FragmentShader;
}

FragmentShader::~FragmentShader()
{
	if (WasSentToPipeline())
		Core::Engine::Get()->GetRenderer()->DeleteResource<FragmentShader>(this);
}

void FragmentShader::Load()
{
	if (IsLoaded())
		return;
	
	sourceCode = LoadShader(name);
	loaded = true;
}

void FragmentShader::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;
	
	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<FragmentShader>(this);
	sentToPipeline = true;
}

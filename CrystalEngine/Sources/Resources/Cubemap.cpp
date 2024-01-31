#include "Resources/Cubemap.h"

#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"

#include <LightJson/JsonElement.h>
#include <LightJson/JsonUtils.h>

using namespace Resources;
using namespace LightJson;

Cubemap::Cubemap(const std::string& _name)
{
	name = _name;
	type = ResourceType::Cubemap;
}

void Cubemap::Load()
{
	loaded = true;
}

void Cubemap::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;
	
	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<Cubemap>(this);
	sentToPipeline = true;
}

void Cubemap::SetCubemapSide(const CubemapSide& side, const std::string& textureName)
{
	sides[(int)side] = textureName;
}

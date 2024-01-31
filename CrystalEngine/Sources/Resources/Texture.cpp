#include "Resources/Texture.h"

#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Debug/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
using namespace Resources;

Texture::Texture(const std::string& _name)
{
	name = _name;
	type = ResourceType::Texture;
}

Texture::~Texture()
{
	if (data) {
		stbi_image_free(data);
		data = nullptr;
	}
	if (WasSentToPipeline()) {
		Core::Engine::Get()->GetRenderer()->DeleteResource<Texture>(this);
	}
}

void Texture::Load()
{
	if (IsLoaded())
		return;
	
	// Load texture data with stbi.
	stbi_set_flip_vertically_on_load_thread(flippedVertically);
	data = stbi_load(name.c_str(), &width, &height, &channels, 0);

	// Make sure loading was successful.
	if (!data) {
		DebugLogError("Unable to load texture: " + name);
		stbi_image_free(data);
		return;
	}
	loaded = true;
}

void Texture::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;

	// Send texture data through the pipeline and delete it from CPU memory.
	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<Texture>(this);
	stbi_image_free(data); data = nullptr;
	sentToPipeline = true;
}

void Texture::SetFlippedVertically(const bool& flipped)
{
	if (flippedVertically == flipped)
		return;

	// Unload the texture.
	const bool wasLoaded     = loaded;
	const bool wasInPipeline = sentToPipeline;
	if (data) {
		stbi_image_free(data);
		data = nullptr;
	}
	if (wasInPipeline) {
		Core::Engine::Get()->GetRenderer()->DeleteResource<Texture>(this);
	}
	loaded = false;
	sentToPipeline = false;

	// Reload the texture.
	flippedVertically = flipped;
	if (wasLoaded    ) Load();
	if (wasInPipeline) SendToPipeline();
}

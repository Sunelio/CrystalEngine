#include "Resources/Font.h"
#include "Core/Engine.h"
#include "Debug/Logger.h"
#include "Render/Renderer.h"
using namespace Resources;

Font::Font(const std::string& _name)
{
	name = _name;
	type = ResourceType::Font;
}

void Font::Load()
{
	if (IsLoaded()) return;

	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft)) {
		DebugLogError("Could not initialize FreeType library.");
		return;
	}

	for (uchar c = 0; c < 128; c++)
	{
		// load font as face
		FT_Face face;
		if (FT_New_Face(ft, name.c_str(), 0, &face)) {
			DebugLogError("Failed to load font " + name + " face " + std::to_string(c));
			continue;
		}

		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			DebugLogError("Failed to load font " + name + " glyph " + std::to_string(c));
			continue;
		}

		faces.push_back(face);
	}
	loaded = true;
}


void Font::SendToPipeline()
{
	if (!IsLoaded() || WasSentToPipeline())
		return;

	Core::Engine::Get()->GetRenderer()->SendResourceToPipeline<Font>(this);
	sentToPipeline = true;
	
	for (uchar c = 0; c < 128; c++)
		FT_Done_Face(faces[c]);
	
	faces.clear();
	FT_Done_FreeType(ft);
}

#pragma once
#include "Maths/Maths.h"
#include"Utils/Typedefs.h"

#include "Resource.h"
#include "../Externals/Includes/freetype/ft2build.h"
#include FT_FREETYPE_H
#include <unordered_map>

namespace Resources
{
	class Character
	{
	private :
		uint           textureID = 0; // ID handle of the glyph texture
		Maths::Vector2 size;          // Size of glyph
		Maths::Vector2 bearing;       // Offset from baseline to left/top of glyph
		uint           advance = 0;   // Horizontal offset to advance to next glyph

	public:
		Character() = default;
		Character(const uint& _id, const Maths::Vector2& _size, const Maths::Vector2& _bearing, const uint& _advance)  : textureID(_id), size(_size), bearing(_bearing), advance(_advance) {}

		uint			GetTextureID() const { return textureID; } // Returns this text character's texture ID.
		Maths::Vector2	GetSize()      const { return size ;     } // Returns this text character's size.
		Maths::Vector2	GetBearing()   const { return bearing;   } // Returns this text character's bearing.
		uint			GetAdvance()   const { return advance;   } // Returns this text character's advance.

	};

	class Font : public Resource
	{
	private:
		std::unordered_map<char, Character> characters;
		std::vector<FT_Face> faces;
		FT_Library ft;
		
	public:
		Font(const std::string& _name);
		
		void Load()	          override; // Loads this font.
		void SendToPipeline() override; // Sends this font to the pipeline.

		void AddCharacter(const char& key, const Character& character) { characters[key] = character; }
		
		const Character& GetCharacter(const char& character) const { return characters.at(character); } // Returns the text character corresponding to the given char.
		const FT_Face&   GetFace     (const uchar& c)        const { return faces[c]; }

		static ResourceType GetResourceType() { return ResourceType::Font; } // Returns the resource type of this class.
	};
}

#pragma once
#include "Utils/Typedefs.h"
#include "Resource.h"
#include "Maths/Vector2.h"

namespace Resources
{
	// - Textures (inherits from Resource) - //
	class Texture : public Resource
	{
	private:
        uint   id    = 0;
		uchar* data  = nullptr;
        int width    = 0;
        int height   = 0;
        int channels = 0;
		bool flippedVertically = true;
		
	public:
		// -- Constructor -- //

		// Creates a texture with the given name (has to be a filename).
		Texture(const std::string& _name);
		~Texture() override;
		Texture(const Texture&)            = delete;
		Texture(Texture&&)                 = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&)      = delete;

		// -- Methods -- //
		void Load()           override; // Loads the texture.
		void SendToPipeline() override; // Sends the texture to GPU memory.
		void SetFlippedVertically(const bool& flipped); // Toggles vertical flip of the texture (triggers a total reload).

		void SetId(const uint& _id) { if (id == 0) id = _id; }

		// -- Getters -- //
		uint           GetId      () const { return id;       } // Returns the texture's pipeline ID.
		int            GetWidth   () const { return width;    } // Returns the texture's width.
		int            GetHeight  () const { return height;   } // Returns the texture's height.
		Maths::Vector2 GetSize    () const { return { (float)width, (float)height }; } // Returns the texture's size.
		uchar*         GetData    () const { return data;     } // Returns the texture's pixel data.
		int            GetChannels() const { return channels; } // Returns the number of color channels in the texture.

		static ResourceType GetResourceType() { return ResourceType::Texture; } // Returns the resource type of this class.
	};
}

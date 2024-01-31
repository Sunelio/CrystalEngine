#pragma once

#include "Resources/Resource.h"
#include "Utils/Typedefs.h"

#include "../Externals/Includes/LightJson/JsonVirtual.h"

namespace Resources
{
	// - CubemapSide  - //
	enum class CubemapSide
	{
		Right,
		Left,
		Top,
		Bottom,
		Front,
		Back,
	};

	// - Cubemap (inherits from Resource) - //
	class Cubemap : public Resource
	{
	public:
		// -- Attributes -- //
		uint id;
		std::string sides[6] = {};

		// -- Constructor -- //

		// Creates a cubemap with the given name (usually a filename).
		Cubemap(const std::string& _name);

		// -- Methods -- //
		void Load()           override; // Loads the cubemap.
		void SendToPipeline() override; // Sends the cubemap to GPU memory.

		void SetCubemapSide(const CubemapSide& side, const std::string& textureName); // Sets the texture name of the given cubemap side to the given filename.

		static ResourceType GetResourceType() { return ResourceType::Cubemap; }
	};
}

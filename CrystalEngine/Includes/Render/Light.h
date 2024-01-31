#pragma once
#include "Maths/Color.h"
#include "Maths/Transform.h"

#include "../Externals/Includes/LightJson/JsonVirtual.h"
namespace LightJson
{
	class JsonObject;
	class JsonElement;
}

namespace Render
{
	// - Light - //
	class Light : public LightJson::JsonVirtual
	{
	private:
		uint id;

	public:
		// -- Attributes -- //
		Maths::Transform* transform = nullptr;         // Pointer to the parent LightObject's transform.
		Maths::RGB ambient, diffuse = 1, specular = 1; // The light's ambient, diffuse and specular colors.
		float constant = 1, linear = 0, quadratic = 0; // The light's constant, linear and quadratic attenuation.
		float outerCutoff = -1, innerCutoff = -1;      // The light's inner and outer cutoffs.

	public:
		Light(const uint& _id) : id(_id) {}
		uint GetId() const { return id; }

		LightJson::JsonElement* Serialize() const                   override;
		void Deserialize(const LightJson::JsonElement* jsonElement) override;
	};
}

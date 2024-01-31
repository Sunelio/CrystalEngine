#pragma once

#include "JsonUtils.h"

namespace LightJson {
	class JsonObject;
	class JsonElement;

	class JsonVirtual
	{
	public:
		virtual ~JsonVirtual() = default;
		
		virtual JsonElement* Serialize  () const { return nullptr; }
		virtual void         Deserialize(const JsonElement* jsonElement) {}
	};
}

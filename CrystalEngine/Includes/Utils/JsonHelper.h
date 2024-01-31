#pragma once

#include "LightJson/JsonUtils.h"

namespace LightJson
{
    class JsonElement;
}

namespace Helper
{
    class JsonHelper
    {
    public:
        template <typename T> static LightJson::JsonElement* Serialize(const T& value);
        template <typename T> static T Deserialize(const LightJson::JsonElement* jsonElement);
    };
}

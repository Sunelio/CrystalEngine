#pragma once

#include "JsonUtils.h"

namespace LightJson
{
    class ArrayElement;
    class JsonElement;
    class JsonObject;

    class JsonDeserializer {
    public:
        static JsonUtils::JsonMap fromJson(const std::string& json);
    
    protected:
        static JsonUtils::JsonPair parseElement     (const std::string& element);
        static ArrayElement*       parseArray       (const std::string& array);
        static JsonElement*        parseArrayElement(const std::string& element);
    };
}

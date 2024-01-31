#pragma once

#include <unordered_map>

namespace LightJson
{
    class JsonElement;

    class JsonUtils
    {
    public:
        typedef std::unordered_map<std::string, JsonElement*> JsonMap;
        typedef std::vector<JsonElement*>                     JsonArray;
        typedef std::pair<std::string, JsonElement*>          JsonPair;

        enum class JsonType {
            Null,
            Boolean,
            Float,
            Integer,
            String,
            Array,
            Object
        };

        static JsonPair makeJsonPair(std::string key, JsonElement* element);
    };
}

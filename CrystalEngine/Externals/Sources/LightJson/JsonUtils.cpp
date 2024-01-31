#include "LightJson/JsonElement.h"

using namespace LightJson;

JsonUtils::JsonPair JsonUtils::makeJsonPair(std::string key, JsonElement* element) {
    return std::make_pair<std::string, JsonElement*>(std::move(key), std::move(element));
}

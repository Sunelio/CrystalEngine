#include "Render/LightManager.h"

#include "LightJson/JsonUtils.h"
#include "LightJson/JsonElement.h"
#include "LightJson/JsonObject.h"
using namespace LightJson;

using namespace Render;

LightManager::~LightManager()
{
    for (uint i = 0; i < MAX_LIGHTS; i++)
    {
        delete lights[i];
        lights[i] = nullptr;
    }
}

Light* LightManager::Create()
{
    for (uint i = 0; i < MAX_LIGHTS; i++)
    {
        if (!lights[i])
        {
            Light* light = new Light(i);
            lights[i] = light;
            return light;
        }
    }
    return nullptr;
}

Light* LightManager::Get(const uint& i) const
{
    if (i > MAX_LIGHTS)
        return nullptr;
    return lights[i];
}

void LightManager::Remove(const Light* light)
{
    const uint id = light->GetId();
    delete lights[id];
    lights[id] = nullptr;
}

void LightManager::Remove(const uint& i)
{
    if (i > MAX_LIGHTS)
        return;
    delete lights[i];
    lights[i] = nullptr;
}

JsonElement* LightManager::Serialize() const
{
    JsonUtils::JsonMap map = {};
    
    JsonUtils::JsonArray lightArray {};
    for (size_t i = 0; i < MAX_LIGHTS; i++)
    {
        if (!lights[i]) continue;
        lightArray.push_back(lights[i]->Serialize());
    }
    map["lights"] = new ArrayElement(lightArray);
    
    return new ObjectElement(map);
}

void LightManager::Deserialize(const JsonElement* jsonElement)
{
    // Clear all lights.
    for (uint i = 0; i < MAX_LIGHTS; i++)
    {
        delete lights[i];
        lights[i] = nullptr;
    }

    // Create all lights from json data.
    const JsonUtils::JsonArray lightArray = jsonElement->getAsObject().at("lights")->getAsArray();
    for (uint i = 0; i < (uint)lightArray.size() && i < MAX_LIGHTS; i++)
    {
        lights[i] = new Light(i);
        lights[i]->Deserialize(lightArray[i]);
    }
}

JsonElement* Light::Serialize() const
{
    JsonUtils::JsonMap map {};
    map["ambient"] = new ArrayElement({
        new NumberElement(ambient.r),
        new NumberElement(ambient.g),
        new NumberElement(ambient.b),
    });
    map["diffuse"] = new ArrayElement({
        new NumberElement(diffuse.r),
        new NumberElement(diffuse.g),
        new NumberElement(diffuse.b),
    });
    map["specular"] = new ArrayElement({
        new NumberElement(specular.r),
        new NumberElement(specular.g),
        new NumberElement(specular.b),
    });
    map["constant"   ] = new NumberElement(constant);
    map["linear"     ] = new NumberElement(linear);
    map["quadratic"  ] = new NumberElement(quadratic);
    map["outerCutoff"] = new NumberElement(outerCutoff);
    map["innerCutoff"] = new NumberElement(innerCutoff);

    return new ObjectElement(map);
}

void Light::Deserialize(const JsonElement* jsonElement)
{
    const JsonUtils::JsonMap   jsonMap      = jsonElement->getAsObject();
    const JsonUtils::JsonArray jsonAmbient  = jsonMap.at("ambient" )->getAsArray();
    const JsonUtils::JsonArray jsonDiffuse  = jsonMap.at("diffuse" )->getAsArray();
    const JsonUtils::JsonArray jsonSpecular = jsonMap.at("specular")->getAsArray();
    ambient  = Maths::RGB(jsonAmbient [0]->getAsFloat(), jsonAmbient [1]->getAsFloat(), jsonAmbient [2]->getAsFloat());
    diffuse  = Maths::RGB(jsonDiffuse [0]->getAsFloat(), jsonDiffuse [1]->getAsFloat(), jsonDiffuse [2]->getAsFloat());
    specular = Maths::RGB(jsonSpecular[0]->getAsFloat(), jsonSpecular[1]->getAsFloat(), jsonSpecular[2]->getAsFloat());

    constant    = jsonMap.at("constant"   )->getAsFloat();
    linear      = jsonMap.at("linear"     )->getAsFloat();
    quadratic   = jsonMap.at("quadratic"  )->getAsFloat();
    outerCutoff = jsonMap.at("outerCutoff")->getAsFloat();
    innerCutoff = jsonMap.at("innerCutoff")->getAsFloat();
}





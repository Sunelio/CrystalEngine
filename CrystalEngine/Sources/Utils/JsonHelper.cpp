#include "Utils/JsonHelper.h"
#include "LightJson/JsonElement.h"
#include "Scenes/ObjectTag.h"
#include "Maths/Transform.h"
#include "Maths/Vector2.h"
#include "Maths/Color.h"
#include "Scenes/GameObject.h"
#include "Physics/RigidbodyVolume.h"
#include "Resources/Sound.h"

using namespace Maths;
using namespace Physic;
using namespace Scenes;
using namespace Geometry3D;

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Vector2& value)
{
    LightJson::JsonUtils::JsonArray array {};
    array.push_back(new LightJson::NumberElement(value.x));
    array.push_back(new LightJson::NumberElement(value.y));
    return new LightJson::ArrayElement(array);
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Vector3& value)
{
    LightJson::JsonUtils::JsonArray array {};
    array.push_back(new LightJson::NumberElement(value.x));
    array.push_back(new LightJson::NumberElement(value.y));
    array.push_back(new LightJson::NumberElement(value.z));
    return new LightJson::ArrayElement(array);
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Quaternion& value)
{
    LightJson::JsonUtils::JsonArray array {};
    array.push_back(new LightJson::NumberElement(value.w));
    array.push_back(new LightJson::NumberElement(value.x));
    array.push_back(new LightJson::NumberElement(value.y));
    array.push_back(new LightJson::NumberElement(value.z));
    return new LightJson::ArrayElement(array);
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const RGBA& value)
{
    LightJson::JsonUtils::JsonArray array {};
    array.push_back(new LightJson::NumberElement(value.r));
    array.push_back(new LightJson::NumberElement(value.g));
    array.push_back(new LightJson::NumberElement(value.b));
    array.push_back(new LightJson::NumberElement(value.a));
    return new LightJson::ArrayElement(array);
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Transform& value)
{
    LightJson::JsonUtils::JsonMap map {};
    map["position"] = Serialize(value.GetPosition());
    map["rotation"] = Serialize(value.GetRotation());
    if (value.GetScale() != Vector3(1)) map["scale" ]   = Serialize(value.GetScale());
    if (value.IsCamera())               map["isCamera"] = new LightJson::BooleanElement(value.IsCamera());
    return new LightJson::ObjectElement(map);
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const ObjectTag& value)
{
    LightJson::JsonUtils::JsonMap map {};
    map["name"] = new LightJson::StringElement(value.name);

    LightJson::JsonUtils::JsonArray array {}; 
    array.reserve(value.tagged.size());
    for (const GameObject* gameObject : value.tagged)
        array.emplace_back(new LightJson::NumberElement(gameObject->GetId()));
    map["tagged"] = new LightJson::ArrayElement(array);

    return new LightJson::ObjectElement(map);
}

template <> Vector2 Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonArray jsonArray = jsonElement->getAsArray();
    return { jsonArray[0]->getAsFloat(), jsonArray[1]->getAsFloat() };
}

template <> Vector3 Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonArray jsonArray = jsonElement->getAsArray();
    return { jsonArray[0]->getAsFloat(), jsonArray[1]->getAsFloat(), jsonArray[2]->getAsFloat() };
}

template <> Quaternion Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonArray jsonArray = jsonElement->getAsArray();
    return { jsonArray[0]->getAsFloat(), jsonArray[1]->getAsFloat(), jsonArray[2]->getAsFloat(), jsonArray[3]->getAsFloat() };
}

template <> RGBA Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonArray jsonArray = jsonElement->getAsArray();
    return { jsonArray[0]->getAsFloat(), jsonArray[1]->getAsFloat(), jsonArray[2]->getAsFloat(), jsonArray[3]->getAsFloat() };
}

template <> Transform Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();
    Transform transform = {
        Deserialize<Vector3   >(jsonMap.at("position")),
        Deserialize<Quaternion>(jsonMap.at("rotation")),
        jsonMap.count("scale")    > 0 ? Deserialize<Vector3>(jsonMap.at("scale")) : Vector3(1),
        jsonMap.count("isCamera") > 0 ? jsonMap.at("isCamera")->getAsBoolean() : false
    };
    return transform;
}

template <> ObjectTag Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap   = jsonElement->getAsObject();
    return ObjectTag{ jsonMap.at("name")->getAsString() };
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Resources::Sound& sound)
{
    LightJson::JsonUtils::JsonMap map {};
    LightJson::JsonUtils::JsonMap properties = {};
    properties["isSpactialized"] = new LightJson::BooleanElement(sound.IsSpatialized());
    properties["isLooping"]      = new LightJson::BooleanElement(sound.IsLooping());
    properties["spatialGainMin"] = new LightJson::NumberElement(sound.GetSpatializationGainMin());
    properties["spatialGainMax"] = new LightJson::NumberElement(sound.GetSpatializationGainMax());
    properties["volume"]         = new LightJson::NumberElement(sound.GetVolume());
    properties["rollOff"]        = new LightJson::NumberElement(sound.GetRollOff());
    properties["innerRadius"]    = new LightJson::NumberElement(sound.GetConeInnerRadius());
    properties["outerRadius"]    = new LightJson::NumberElement(sound.GetConeOuterRadius());
    properties["gain"]           = new LightJson::NumberElement(sound.GetConeGain());
    properties["dopplerFactor"]  = new LightJson::NumberElement(sound.GetDopplerFactor());
    properties["fadeIn"]         = new LightJson::NumberElement(sound.GetFadeIn());
    properties["fadeOut"]        = new LightJson::NumberElement(sound.GetFadeOut());

    map[sound.GetName()] = new LightJson::ObjectElement(properties);
    return new LightJson::ObjectElement(map);
}

template <> Resources::Sound Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();
    return Resources::Sound("EW");
}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const Sphere& sphere)
{
    LightJson::JsonUtils::JsonArray array{};
    array.push_back(Serialize(sphere.position));
    array.push_back(new LightJson::NumberElement(sphere.radius));
    return new LightJson::ArrayElement(array);
}

template <> Sphere Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();

    Sphere sphere =
    {
        Deserialize<Vector3>(jsonMap.at("position")),
        jsonMap.at("radius")->getAsFloat()
    };
    return sphere;

}

template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const OBB& box)
{
    LightJson::JsonUtils::JsonArray array{};
    array.push_back(Serialize(box.position));
    array.push_back(Serialize(box.size));
    return new LightJson::ArrayElement(array);
}

template <> OBB Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();

    OBB obb = 
    {
        Deserialize<Vector3   >(jsonMap.at("position")),
        Deserialize<Vector3 >(jsonMap.at("size"))
    };
    return obb;

}


#if 0
template <> LightJson::JsonElement* Helper::JsonHelper::Serialize(const RigidbodyVolume& rigidbody)
{
    LightJson::JsonUtils::JsonArray array{};
    array.push_back(Serialize(rigidbody.owner));
    array.push_back(new LightJson::NumberElement(rigidbody.type));
    array.push_back(new LightJson::NumberElement(rigidbody.takeGravity));
    array.push_back(new LightJson::NumberElement(rigidbody.isTrigger));
    array.push_back(Serialize(rigidbody.forces));
    array.push_back(Serialize(rigidbody.position));
    array.push_back(Serialize(rigidbody.velocity));
    array.push_back(Serialize(rigidbody.angVelocity));
    array.push_back(Serialize(rigidbody.orientation));
    array.push_back(new LightJson::NumberElement(rigidbody.cor)); //NOTE(o.luanda): Coefficient of restituition
    array.push_back(Serialize(rigidbody.orientation));
    array.push_back(Serialize(rigidbody.box));
    array.push_back(Serialize(rigidbody.sphere));

    return new LightJson::ArrayElement(array);
}

template <> RigidbodyVolume Helper::JsonHelper::Deserialize(const LightJson::JsonElement* jsonElement)
{
    const LightJson::JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();

    RigidbodyVolume rigidbody =
    {
        Deserialize<GameObject*>(jsonMap.at("owner")),
        jsonMap.at("type")->getAsInteger()
    };

    return rigidbody;

}
#endif
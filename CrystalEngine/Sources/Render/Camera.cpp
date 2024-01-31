#include "Render/Camera.h"
#include "Core/Engine.h"
#include "Core/EventDispatcher.h"
#include "Debug/Logger.h"

#include <LightJson/JsonElement.h>
#include <LightJson/JsonUtils.h>

#include "Utils/IdGenerator.h"
using namespace LightJson;

using namespace Render;
using namespace Core;
using namespace Utils;

Camera::Camera(const CameraParams& _params, const size_t& _id) : id (_id)
{
    SetParams(_params);
    resizeFunc = [this](Event* event)
    {
        const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
        SetParams({ (unsigned int)resizeEvent->width, (unsigned int)resizeEvent->height, params.near, params.far, params.fov });
    };
}

Camera::Camera(const int& width, const int& height, const size_t& _id) : id (_id)
{
    params.height = height;
    params.width  = width;
    SetParams(params);
    resizeFunc = [this](Event* event)
    {
        const ScreenResizeEvent* resizeEvent = (ScreenResizeEvent*)event;
        SetParams({ (unsigned int)resizeEvent->width, (unsigned int)resizeEvent->height, params.near, params.far, params.fov });
    };
}

Camera::~Camera()
{
    Engine::Get()->GetEventDispatcher()->RemoveListener<ScreenResizeEvent>(resizeFunc);
}

JsonElement* Camera::Serialize() const
{
    JsonUtils::JsonMap map = {};
    map["id"    ] = new NumberElement(id);
    map["width" ] = new NumberElement(params.width);
    map["height"] = new NumberElement(params.height);
    map["near"  ] = new NumberElement(params.near);
    map["far"   ] = new NumberElement(params.far);
    map["fov"   ] = new NumberElement(params.fov);
    
    return new ObjectElement(map);
}

void Camera::Deserialize(const JsonElement* jsonElement)
{
    const JsonUtils::JsonMap jsonMap = jsonElement->getAsObject();
    SetParams({
        (unsigned int)jsonMap.at("width" )->getAsInteger(),
        (unsigned int)jsonMap.at("height")->getAsInteger(),
        jsonMap.at("near"  )->getAsFloat(),
        jsonMap.at("far"   )->getAsFloat(),
        jsonMap.at("fov"   )->getAsFloat()
    });
}

void Camera::SetParams(const CameraParams& _params)
{
    params = _params;
    const float tanHalfFov = tan(Maths::degToRad(params.fov * 0.5f));
    projectionMat = Maths::Mat4(
        1 / (params.aspect * tanHalfFov), 0, 0, 0,
        0, 1 / tanHalfFov, 0, 0,
        0, 0, -(params.far + params.near) / (params.far - params.near), -1,
        0, 0, -(2 * params.far * params.near) / (params.far - params.near), 0
    );
}

void Camera::SetFullscreen(const bool& fullscreen) const
{
    if (fullscreen) Engine::Get()->GetEventDispatcher()->AddListener   <ScreenResizeEvent>(resizeFunc);
    else            Engine::Get()->GetEventDispatcher()->RemoveListener<ScreenResizeEvent>(resizeFunc);
}

Maths::Mat4 Camera::GetViewMat() const
{
    if (!transform)
    {
        DebugLogWarning("Tried to get view matrix of camera without transform.");
        return Maths::Mat4();
    }
    return transform->GetViewMat();
}

#pragma once
#include "Maths/Transform.h"
#include <functional>

#include "../Externals/Includes/LightJson/JsonVirtual.h"

namespace Core { struct Event; }
namespace Render
{
    // - CameraParams: holds camera parameters - //
    struct CameraParams
    {
        int width = 0, height = 0;
        float near = 0.01f, far = 100.f, fov = 60.f;
        float aspect = 1.f;

        CameraParams() {}
        CameraParams(const unsigned int& _width, const unsigned int& _height)
            : width(_width), height(_height), near(), far(), fov(), aspect((float)width/(float)height) {}
        CameraParams(const unsigned int& _width, const unsigned int& _height, const float& _near, const float& _far, const float& _fov)
            : width(_width), height(_height), near(_near), far(_far), fov(_fov), aspect((float)width/(float)height) {}
    };

    // - Camera - //
    class Camera : public LightJson::JsonVirtual
    {
    private:
        size_t id;
        CameraParams params;
        Maths::Mat4  projectionMat;
        std::function<void(Core::Event*)> resizeFunc;
        
    public:
        // -- Attributes -- //
        Maths::Transform* transform = nullptr; // Pointer to the parent CameraObject's transform.

        // -- Constructors -- //
        Camera(const CameraParams& _params, const size_t& _id);         // Create a camera from the given parameters.
        Camera(const int& width, const int& height, const size_t& _id); // Create a camera from the given width and height.
        ~Camera() override;
        
        // -- Setters -- //
        void SetParams(const CameraParams& _params);       // Modifies the camera's parameters to the given ones.
        void SetFullscreen(const bool& fullscreen) const;  // Toggles camera fullscreen (if true, the camera's size matches the window size).

        // -- Getters -- //
        CameraParams GetParams()         const { return params; }        // Returns the camera's parameters.
        Maths::Mat4  GetProjectionMat()  const { return projectionMat; } // Returns the camera's perspective projection matrix.
        Maths::Mat4  GetViewMat()        const;                          // Returns the camera's view transformation matrix.
        size_t       GetId()             const { return id; }            // Returns the camera's ID.
        
        LightJson::JsonElement* Serialize() const override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}

#pragma once
#include <vector>

#include "Camera.h"
#include "Utils/IdGenerator.h"
#include "../Externals/Includes/LightJson/JsonVirtual.h"

namespace Core { struct WindowInputs; }
namespace Render
{
    struct CameraParams;
    class  Camera;

    // - CameraManager - //
    class CameraManager : public LightJson::JsonVirtual
    {
    private:
        std::vector<Camera*> cameras;
        Camera*              sceneCamera  = nullptr;
		Camera*              engineCamera = nullptr;
        float engineCameraSpeed       = 2;
        float engineCameraSensitivity = 1.5f;

        Utils::IdGenerator idGenerator;
    
    public:
        CameraManager();
        ~CameraManager();
        CameraManager(const CameraManager&)            = delete;
        CameraManager(CameraManager&&)                 = delete;
        CameraManager& operator=(const CameraManager&) = delete;
        CameraManager& operator=(CameraManager&&)      = delete;

        // -- Handling Cameras -- //
        Camera* Create(const CameraParams& params);   // Creates a new camera from the given parameters.
        Camera* Get   (const size_t&       id) const; // Returns the camera with the given ID.
        void Delete   (const size_t&       id);       // Deletes the camera with the given ID.
        void Delete(const Camera* camera);   // Deletes the given camera.
        
        // -- Miscellaneous -- //
        void UpdateEngineCamera(const float& deltaTime, const Core::WindowInputs& inputs) const; // Updates the transform of the edit mode camera based on user input.
        void SetSceneCamera(Camera* camera); // Changes which camera is used to render the scene in play mode.
        
        // -- Getters -- //
        Camera* GetEngineCamera() const { return engineCamera; } // Returns the camera from which the scene is rendered in edit mode.
        Camera* GetSceneCamera () const { return sceneCamera;  } // Returns the camera from which the scene is rendered in play mode.
        std::vector<Camera*> GetCameras() const { return cameras; } // Returns all of the cameras in this manager.
        
        LightJson::JsonElement* Serialize() const override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}

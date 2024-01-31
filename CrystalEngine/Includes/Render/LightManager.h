#pragma once
#include "Utils/Typedefs.h"
#include "Light.h"

namespace Render
{
    // - LightManager - //
    class LightManager : public LightJson::JsonVirtual
    {
    public:
        // Maximum number of lights that can exist in a scene.
        inline static constexpr uint MAX_LIGHTS = 30;
        
    private:
        Light* lights[MAX_LIGHTS] = {};
        
    public:
        LightManager()                               = default;
        ~LightManager() override;
        LightManager(const LightManager&)            = delete;
        LightManager(LightManager&&)                 = delete;
        LightManager& operator=(const LightManager&) = delete;
        LightManager& operator=(LightManager&&)      = delete;
        
        // -- Handling Lights -- //
        Light*       Create();                            // Creates a new light and returns it.
        Light*       Get(const uint& i) const;            // Returns the light at the given index (might be nullptr).
        Light*const* GetLights() const { return lights; } // Returns the entire array of lights (length is MAX_LIGHTS).
        void         Remove(const Light* light);          // Removes the given light.
        void         Remove(const uint& i);               // Removes the light at the given index.

        LightJson::JsonElement* Serialize() const                   override;
        void Deserialize(const LightJson::JsonElement* jsonElement) override;
    };
}

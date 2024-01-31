#pragma once
#include <chrono>

namespace Core
{
    // - TimeManager: handles FPS, delta time and vertical synchronization - //
    class TimeManager
    {
    private:
        int   targetFPS;
        bool  vsync;
        float targetDeltaTime, lastFrameDuration;
        std::chrono::nanoseconds targetDeltaNanoSeconds;
        std::chrono::steady_clock::time_point frameStartTime;
        
    public:
        // -- Constructor -- //
        TimeManager(const int& _targetFPS, const bool& _vsync); // Parameters for maximum frames per second and state of vertical synchronization.
        ~TimeManager()                             = default;
        TimeManager(const TimeManager&)			   = delete;
        TimeManager(TimeManager&&)				   = delete;
        TimeManager& operator=(const TimeManager&) = delete;
        TimeManager& operator=(TimeManager&&)      = delete;
        
        // -- Setters -- //
        void  SetVsyncEnabled(const bool& _vsync);     // Enables/disables vertical synchronization.
        void  SetTargetFPS   (const int&  _targetFPS); // Modifies the maximum frames per second.

        // -- Getters -- //
        bool  IsVsyncEnabled() const { return vsync;     } // Returns true if vertical synchronization is enabled.
        int   GetTargetFPS  () const { return targetFPS; } // Returns the maximum frames per second.
        float FPS      () const; // Return the current frames per second.
        float DeltaTime() const; // Return the current delta time.

        // -- Miscellaneous -- //
        bool CanStartNextFrame() const;  // Returns true if the next frame can be started;
        void NewFrame();                 // Updates FPS and delta time for the current frame.
    };
}

#include "Core/TimeManager.h"

#include <thread>

#include "Core/Engine.h"
#include "Core/Window.h"
using namespace Core;
using namespace std::chrono;

TimeManager::TimeManager(const int& _targetFPS, const bool& _vsync)
    : targetFPS(_targetFPS), vsync(_vsync), targetDeltaTime(0), lastFrameDuration(0)
{
    frameStartTime = steady_clock::now();
    SetTargetFPS(_targetFPS);
    SetVsyncEnabled(_vsync);
}

void TimeManager::SetVsyncEnabled(const bool& _vsync)
{
    vsync = _vsync;
    if (const Engine* engine = Engine::Get())
        if (const Window* window = engine->GetWindow())
            window->SetVsync(_vsync);
}

void TimeManager::SetTargetFPS(const int& _targetFPS)
{
    targetFPS = _targetFPS;
    targetDeltaTime = 1.f / targetFPS;
    targetDeltaNanoSeconds = nanoseconds{(int)(targetDeltaTime*1e9)};
}

float TimeManager::FPS() const
{
    return 1 / DeltaTime();
}

float TimeManager::DeltaTime() const
{
    return lastFrameDuration;
}

bool TimeManager::CanStartNextFrame() const
{
    return steady_clock::now() - frameStartTime > targetDeltaNanoSeconds;
}

void TimeManager::NewFrame()
{
    const steady_clock::time_point lastFrameTime = frameStartTime;
    frameStartTime    = steady_clock::now();
    lastFrameDuration = (float)(duration_cast<nanoseconds>(frameStartTime - lastFrameTime).count() * 1e-9);
}

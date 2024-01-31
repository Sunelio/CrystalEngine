#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"

#include "Render/CameraManager.h"

#include "Debug/Logger.h"

#include "Resources/Sound.h"

#include "Core/SoundManager.h"
#include "Core/Engine.h"

#include "Maths/Transform.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include "Resources/ResourceManager.h"

using namespace Resources;
using namespace Core;

SoundManager::SoundManager()
{
	engine = new ma_engine();
	const ma_result result = ma_engine_init(NULL, engine);
	if(result != MA_SUCCESS) {
		DebugLogError("Failed to initialize sound manager.");
	}

	SetListenersCount(1);
}

SoundManager::~SoundManager()
{
	ma_engine_uninit(engine);
	engine = nullptr;
}

void SoundManager::SetListenersCount(const int& count) const
{
}

void SoundManager::SetListenerPosition(const int& index, Maths::Vector3* position) const
{
	ma_engine_listener_set_position(engine, index, position->x, position->y, position->z);
}

void SoundManager::SetListenerDirection(const int& index, Maths::Vector3* direction) const
{
	ma_engine_listener_set_direction(engine, index, direction->x, direction->y, direction->z);
}

void SoundManager::SetListenerArea(const int& index, const float& innerRadius, const float& outerRadius, const float& gain) const
{
	ma_engine_listener_set_cone(engine, index, innerRadius, outerRadius, gain);
}
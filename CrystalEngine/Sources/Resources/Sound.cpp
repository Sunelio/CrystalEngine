#include "Resources/Sound.h"
#include "Debug/Logger.h"

#include "miniaudio/miniaudio.h"

#include "Core/Engine.h"
#include "Core/SoundManager.h"

#include <iostream>

using namespace Resources;

Sound::Sound(const std::string& _name)
{
	name   = _name;
	type   = ResourceType::Sound;
	engine = Core::Engine::Get()->GetSoundManager()->GetEngine();

	sound = new ma_sound();
	const ma_result result = ma_sound_init_from_file(engine, name.c_str(), 0, NULL, NULL, sound);
	if(result != MA_SUCCESS)
	{
		DebugLogError("Failed to initialize sound: " + name);
	}
	else 
	{
		DebugLogInfo("Successfully loaded sound: " + name);
	}

	soundManager = Core::Engine::Get()->GetSoundManager();
}

Sound::~Sound()
{
	engine = nullptr;
}

void Sound::SendToPipeline()
{
	sentToPipeline = true;
}

void Sound::Start()
{
	isPlaying = true;
	ma_sound_start(sound);
}

void Sound::ScheduleStart(const int& time) const
{
	ma_sound_start(sound);
	ma_sound_set_start_time_in_milliseconds(sound, ma_engine_get_time(engine) + (ma_engine_get_sample_rate(engine) * time));
}

void Sound::Stop()
{
	isPlaying = false;
	ma_sound_stop(sound);
}

void Sound::SetSpatialization(const bool& condition)
{
	isSpatialized = condition;
	ma_sound_set_spatialization_enabled(sound, condition);
}

void Sound::SetSpatializationGains(const float& min, const float& max)
{
	SetSpatializationMinGain(min);
	SetSpatializationMaxGain(max);
}

void Sound::SetSpatializationMinGain(const float& min)
{
	spatialGainMin = min;
	ma_sound_set_min_gain(sound, min);
}

void Sound::SetSpatializationMaxGain(const float& max)
{
	spatialGainMax = max;
	ma_sound_set_max_gain(sound, max);
}

void Sound::SetVolume(const float& _volume)
{
	volume = _volume;
	ma_sound_set_volume(sound, volume);
}

void Sound::SetPosition(const Maths::Vector3& position) const
{
	ma_sound_set_position(sound, position.x, position.y, position.z);
}

void Sound::SetDirection(const Maths::Vector3& direction) const
{
	ma_sound_set_direction(sound, direction.x, direction.y, direction.z);
}

void Sound::SetVelocity(const Maths::Vector3& velocity) const
{
	ma_sound_set_velocity(sound, velocity.x, velocity.y, velocity.z);
}

void Sound::SetArea(const float& _innerRadius, const float& _outerRadius, const float& _gain)
{
	innerRadius = _innerRadius; outerRadius = _outerRadius; gain = _gain;
	ma_sound_set_cone(sound, innerRadius, outerRadius, gain);
}

void Sound::SetAreaInnerRadius(const float& _innerRadius)
{
	innerRadius = _innerRadius;
	ma_sound_set_cone(sound, innerRadius, outerRadius, gain);
}

void Sound::SetAreaOuterRadius(const float& _outerRadius)
{
	outerRadius = _outerRadius;
	ma_sound_set_cone(sound, innerRadius, outerRadius, gain);
}

void Sound::SetAreaGain(const float& _gain)
{
	gain = _gain;
	ma_sound_set_cone(sound, innerRadius, outerRadius, gain);
}

void Sound::SetRollOff(const float& _rollOff)
{
	rollOff = _rollOff;
	ma_sound_set_rolloff(sound, rollOff);
}

void Sound::SetDopplerFactor(const float& factor)
{
	dopplerFactor = factor;
	ma_sound_set_doppler_factor(sound, factor);
}

void Sound::SetFades(const float& in, const float& out)
{
	SetFadeIn(in);
	SetFadeOut(out);
}

void Sound::SetFadeIn(const float& in) 
{
	fadeIn = in;
	ma_sound_set_fade_in_milliseconds(sound, 0, in, 1000);
}

void Sound::SetFadeOut(const float& out)
{
	fadeOut = out;
	ma_sound_set_fade_in_milliseconds(sound, -out, 0, 1000);
}


void Sound::SetLooping(const bool& condition)
{
	isLooping = condition;
	ma_sound_set_looping(sound, condition);
}

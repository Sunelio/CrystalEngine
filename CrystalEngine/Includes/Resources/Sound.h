#pragma once

#include "Resource.h"
#include "Maths/Vector3.h"

struct ma_engine;
struct ma_sound;

namespace Core { class SoundManager; }

namespace Resources
{
	// - Sound (inherits from Resource) - //
	class Sound : public Resource
	{
	private:
		// -- Attributes -- //
		ma_sound*  sound;
		ma_engine* engine;
		
		bool  isPlaying		 = false;
		bool  isLooping      = false;
		bool  isSpatialized  = false;
		float volume		 = 0.f;
		float spatialGainMin = 0.f;
		float spatialGainMax = 0.f;
		float rollOff        = 0.f;
		float innerRadius    = 0.f;
		float outerRadius    = 0.f; 
		float gain		     = 0.f;
		float dopplerFactor  = 0.f;
		float fadeIn		 = 0.;
		float fadeOut	     = 0.f;

		Maths::Vector3  position;
		Maths::Vector3  direction;
		Maths::Vector3  velocity;

		Core::SoundManager* soundManager = nullptr;

	public:
		// -- Constructor -- //
		Sound(const std::string& _name);
		~Sound()					   override;
		Sound(const Sound&)			   = delete;
		Sound(Sound&&)				   = delete;
		Sound& operator=(const Sound&) = delete;
		Sound& operator=(Sound&&)      = delete;

		// -- Methods -- //
		void Load()           override {}
		void SendToPipeline() override;

		void Start();
		void ScheduleStart(const int& time) const; // timestamp in ms.
		void Stop();

		// -- Setters -- //
		void SetLooping              (const bool& condition);
		void SetSpatialization       (const bool& condition);
		void SetSpatializationGains  (const float& min, const float& max);
		void SetSpatializationMinGain(const float& min);
		void SetSpatializationMaxGain(const float& max);

		void SetVolume			     (const float& volume);

		void SetPosition             (const Maths::Vector3& position)  const;
		void SetDirection            (const Maths::Vector3& direction) const;
		void SetVelocity             (const Maths::Vector3& velocity)  const;

		void SetArea                 (const float& _innerRadius, const float& _outerRadius, const float& _gain);
		void SetAreaInnerRadius      (const float& _innerRadius);
		void SetAreaOuterRadius      (const float& _outerRadius);
		void SetAreaGain             (const float& _gain);

		void SetVelocity             (const Maths::Vector3& velocity);
		void SetRollOff              (const float& _rollOff);
		void SetDopplerFactor        (const float& factor);
		void SetFades                (const float& in, const float& out);
		void SetFadeIn               (const float& in);
		void SetFadeOut              (const float& out);

		// -- Getters -- //
		ma_sound* GetSound()			 const { return sound; }

		bool  IsPlaying()			     const { return isPlaying; }
		bool  IsSpatialized()			 const { return isSpatialized; }
		bool  IsLooping()				 const { return isLooping; }

		float GetSpatializationGainMin() const { return spatialGainMin; }
		float GetSpatializationGainMax() const { return spatialGainMax; }
		float GetVolume()				 const { return volume; }
		float GetRollOff()			     const { return rollOff; }
		float GetConeInnerRadius()		 const { return innerRadius; }
		float GetConeOuterRadius()		 const { return outerRadius; }
		float GetConeGain()				 const { return gain; }
		float GetDopplerFactor()		 const { return dopplerFactor; }
		float GetFadeIn()				 const { return fadeIn; }
		float GetFadeOut()				 const { return fadeOut; }

		Maths::Vector3 GetPosition()	 const { return velocity; }
		Maths::Vector3 GetDirection()	 const { return velocity; }
		Maths::Vector3 GetVelocity()	 const { return velocity; }

		Core::SoundManager* GetSoundManager() const { return soundManager; }
		
		static ResourceType GetResourceType() { return ResourceType::Sound; }
	};
}

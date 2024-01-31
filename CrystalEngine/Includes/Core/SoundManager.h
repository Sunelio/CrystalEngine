#pragma once

#include <unordered_map>

#include "Maths/Maths.h"

namespace Resources { class Sound; }

struct ma_engine;

namespace Core
{
	class SoundManager
	{
	private:
		ma_engine* engine;

	public:
		SoundManager();
		~SoundManager();

		void SetListenersCount(const int& count) const;
		void SetListenerPosition(const int& index, Maths::Vector3* position) const;
		void SetListenerDirection(const int& index, Maths::Vector3* direction) const;
		void SetListenerArea(const int& index, const float& innerRadius, const float& outerRadius, const float& gain) const;
		
		ma_engine* GetEngine() const { return engine; }
	};
}

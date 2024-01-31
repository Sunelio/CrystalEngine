#pragma once
#include <typeindex>

namespace Resources { class Resource; }
namespace Core
{
	// - Event: parent class of all custom events - //
	struct Event {};

	// - DelayedEvent: holds an event until the frames or seconds delays are elapsed - //
	struct DelayedEvent
	{
		Event* event            = nullptr;
		std::type_index typeIdx = typeid(void);
		float secondsDelay      = -1;
		int   framesDelay       = -1;

		DelayedEvent() = default;
		template <typename T> DelayedEvent(T* _event, const float& seconds = -1, const int& frames = -1)
			: event((Event*)_event), typeIdx(typeid(T)), secondsDelay(seconds), framesDelay(frames) {}
	};

	struct ScreenResizeEvent : Event
	{
		const int width, height;
		ScreenResizeEvent(const int& w, const int& h) : width(w), height(h) {}
	};
	struct ReloadSceneEvent : Event
	{
	};
	struct SetActiveSceneEvent : Event
	{
		const std::string name;
		const bool shouldStart;
		SetActiveSceneEvent(std::string n, const bool& start) : name(std::move(n)), shouldStart(start) {}
	};
	struct ResourceLoadedEvent : Event
	{
		Resources::Resource* resource;
		ResourceLoadedEvent(Resources::Resource* _resource) : resource(_resource) {}
	};
	struct RemoveGameObjectEvent : Event
	{
		const size_t id;
		RemoveGameObjectEvent(const size_t& _id) : id(_id) {}
	};
	struct RemoveWidgetEvent : Event
	{
		const size_t id;
		RemoveWidgetEvent(const size_t& _id) : id(_id) {}
	};
}

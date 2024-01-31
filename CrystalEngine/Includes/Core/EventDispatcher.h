#pragma once
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <atomic>

namespace Core
{
    struct Event;
    struct DelayedEvent;

	// - EventDispatcher: handles custom events and callbacks - //
	class EventDispatcher
    {
    private:
        std::unordered_map<std::type_index, std::vector<std::function<void(Event*)>>> listeners;
    	std::vector<DelayedEvent> eventQueue;
		std::atomic_flag queueLock = ATOMIC_FLAG_INIT;

    public:
    	EventDispatcher()                                  = default;
    	~EventDispatcher()                                 = default;
        EventDispatcher(const EventDispatcher&)            = delete;
        EventDispatcher(EventDispatcher&&)                 = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;
        EventDispatcher& operator=(EventDispatcher&&)      = delete;
        
        template<typename T> void AddListener   (const std::function<void(Event*)>& listener); // Adds the given listener to the template event.
        template<typename T> void RemoveListener(const std::function<void(Event*)>& listener); // Removes the given listener from the template event.

    	template<typename T> void Dispatch             (T* event);                            // Notifies all listeners of the template event.
        template<typename T> void FutureDispatchSeconds(T* event, const float& secondsDelay); // Notifies all listeners of the template event after the given delay in seconds.
        template<typename T> void FutureDispatchFrames (T* event, const int&   framesDelay ); // Notifies all listeners of the template event after the given delay in frames.
		template<typename T> void FrameEndDispatch     (T* event);                            // Notifies all listeners of the template event at the end of the frame.

    	void Update(const float& deltaTime); // Updates latent events and calls them when they are ready.

    private:
		template<typename T> bool IsEventType() const;
    	void Dispatch(Event* event, const std::type_index& typeIdx);
    };
}

#include "EventDispatcher.inl"

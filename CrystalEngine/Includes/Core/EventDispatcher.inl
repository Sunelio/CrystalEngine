#pragma once
#include "EventDispatcher.h"
#include "Events.h"
#include <algorithm>

namespace Core
{
    template<typename T> void EventDispatcher::AddListener(const std::function<void(Event*)>& listener)
    {
        if (!IsEventType<T>()) return;
        const std::type_index typeIdx = typeid(T);
        if (listeners.count(typeIdx) <= 0)
            listeners[typeIdx] = { listener };
        else
            listeners[typeIdx].emplace_back(listener);
    }

    template<typename T> void EventDispatcher::RemoveListener(const std::function<void(Event*)>& listener)
    {
        if (!IsEventType<T>()) return;
        const std::type_index typeIdx = typeid(T);
        if (listeners.count(typeIdx) > 0)
        {
            std::vector<std::function<void(Event*)>>& typeListeners = listeners[typeIdx];
            for (size_t i = typeListeners.size()-1; i+1 > 0; --i)
            {
                if (typeListeners[i].target_type() == listener.target_type() && typeListeners[i].target<void(Event*)>() == listener.target<void(Event*)>())
                {
                    typeListeners.erase(typeListeners.begin()+i);
                }
            }
        }
    }

    template<typename T> void EventDispatcher::Dispatch(T* event)
    {
        if (!IsEventType<T>()) return;
        const std::type_index typeIdx = typeid(T);
        Dispatch((Event*)event, typeIdx);
    }

    template<typename T> void EventDispatcher::FutureDispatchSeconds(T* event, const float& secondsDelay)
    {
        if (!IsEventType<T>()) return;
        while (queueLock.test_and_set()) {}
        eventQueue.push_back(DelayedEvent(event, secondsDelay, -1));
        queueLock.clear();
    }
    template<typename T> void EventDispatcher::FutureDispatchFrames(T* event, const int& framesDelay)
    {
        if (!IsEventType<T>()) return;
        while (queueLock.test_and_set()) {}
        eventQueue.push_back(DelayedEvent(event, -1.f, framesDelay));
        queueLock.clear();
    }
    template <typename T> void EventDispatcher::FrameEndDispatch(T* event)
    {
        if (!IsEventType<T>()) return;
        while (queueLock.test_and_set()) {}
        eventQueue.push_back(DelayedEvent(event, -1.f, 0));
        queueLock.clear();
    }

    inline void EventDispatcher::Update(const float& deltaTime)
    {
        for (int i = (int)eventQueue.size()-1; i >= 0; i--)
        {
            DelayedEvent& delayedEvent = eventQueue[i];

            // Dispatch events whose timers have ran out.
            if (delayedEvent.framesDelay <= 0 && delayedEvent.secondsDelay <= 0)
            {
                Dispatch(delayedEvent.event, delayedEvent.typeIdx);
                while (queueLock.test_and_set()) {}
                eventQueue.erase(eventQueue.begin()+i);
                queueLock.clear();
                continue;
            }
            
            // Update delay timers.
            if (delayedEvent.secondsDelay > 0)
                delayedEvent.secondsDelay -= deltaTime;
            if (delayedEvent.framesDelay > 0)
                delayedEvent.framesDelay--;
        }
    }

    template <typename T> bool EventDispatcher::IsEventType() const
    {
        return std::is_base_of_v<Event, T>;
    }
    
    inline void EventDispatcher::Dispatch(Event* event, const std::type_index& typeIdx)
    {
        if (!event || listeners.count(typeIdx) <= 0) return;
        for (size_t i = 0; i < listeners[typeIdx].size(); i++)
        {
            listeners[typeIdx][i](event);
        }
        delete event;
    }
}

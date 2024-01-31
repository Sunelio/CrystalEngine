#include "Core/ThreadManager.h"

#include <iostream>
#include <ostream>
#include <string>
using namespace Core;
using namespace std::chrono_literals;

void ThreadManager::AddTask(std::function<void()>& task)
{
    while (lock.test_and_set()) {}
    if(!task) return;
    
    tasks.push_back(std::move(task));
    lock.clear();
}

void ThreadManager::Update()
{
    // Create threads for all tasks.
    while (!tasks.empty() && threads.size() < threadCount)
    {
        const auto& task = tasks[0];
        threads.emplace_back(std::async(std::launch::async, task));
        tasks.erase(tasks.begin());
    }

    // Delete threads that are finished.
    for (int i = (int)threads.size()-1; i >= 0; i--)
    {
        if (threads[i].wait_for(0ms) == std::future_status::ready)
            threads.erase(threads.begin() + i);
    }
}

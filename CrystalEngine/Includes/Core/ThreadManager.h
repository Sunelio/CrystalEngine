#pragma once
#include <atomic>
#include <vector>
#include <future>
#include <functional>

namespace Core
{
	// - ThreadManager: handles multithreading through a task system - //
	class ThreadManager
	{
	private:
		static constexpr size_t threadCount = 15;
		std::atomic_flag lock = ATOMIC_FLAG_INIT;
		std::vector<std::future<void>>     threads;
		std::vector<std::function<void()>> tasks;

	public:
		ThreadManager () = default;
		~ThreadManager() = default;
		ThreadManager(const ThreadManager&)            = delete;
		ThreadManager(ThreadManager&&)                 = delete;
		ThreadManager& operator=(const ThreadManager&) = delete;
		ThreadManager& operator=(ThreadManager&&)      = delete;

		void AddTask(std::function<void()>& task); // Adds a new task to the list.
		void Update();                             // Check for new tasks and dispatch threads.
	};
}

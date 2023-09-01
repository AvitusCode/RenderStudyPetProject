#pragma once
#include "ThreadQueue.h"
#include <thread>
#include <future>
#include <functional>
#include <utility>
#include <vector>
#include <memory>
#include "ECS/Types.h"

namespace jd::utils
{
	class ThreadPool final
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;
		~ThreadPool();

		static ThreadPool& Instance() {
			static ThreadPool tp{ THREADS_NUM };
			return tp;
		}

		// Blocking execution in another thread while waiting for the result
		template<typename F, typename... Args>
		auto submit(F&& f, Args&&... args)
		{
			using RawType = std::invoke_result_t<F, Args...>;
			std::function<RawType()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
			auto task = std::make_shared<std::packaged_task<RawType()>>(func);
			auto future = task->get_future();

			std::function<void()> func_wrapper = [task = std::move(task)]() {
				(*task)();
			};

			m_queue.push(func_wrapper);
			m_cv.notify_one();

			return future;
		}

		// Independent execution of a function in another thread
		template<typename F, typename... Args>
		void postToThread(F&& f, Args&&... args)
		{
			using RawType = std::invoke_result_t<F, Args...>;
			std::function<RawType()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

			std::function<void()> func_wrapper = [callback = std::move(func)]() {
				callback();
			};

			m_queue.push(func_wrapper);
			m_cv.notify_one();
		}

	private:
		explicit ThreadPool(size_t n_threads);
		// Inits thread pool
		void Init();
		// Waits until threads finish their current task and shutdowns the pool
		void shutdown();

		class Worker
		{
		public:
			Worker(const size_t id, ThreadPool* const pool) : id_(id), p_pool(pool) {};

			void operator()() {
			std::function<void()> func;
			bool dequeued = false;

			while (!p_pool->m_shutdown) {
				{
					std::unique_lock<std::mutex> lk(p_pool->m_conditional_mutex);
					p_pool->m_cv.wait(lk, [this]() {
						return !p_pool->m_queue.empty() || p_pool->m_shutdown;
						});
					
					dequeued = p_pool->m_queue.dequeue(func);
				}

				if (dequeued) {
					func();
				}
			}
		}

		private:
			int id_;
			ThreadPool* p_pool;
		};

		//@data
		bool m_init;
		bool m_shutdown;
		ThreadQueue<std::function<void()>> m_queue;
		std::vector<std::thread> m_threads;
		std::mutex m_conditional_mutex;
		std::condition_variable m_cv;
	};
}
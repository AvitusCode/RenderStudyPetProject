#include "Utils/ThreadPool.h"
#include "Utils/logger.h"
#define MIN(a, b) (a) < (b) ? (a) : (b)

namespace jd::utils
{
	ThreadPool::ThreadPool(size_t n_threads) : 
		m_init(false), 
		m_shutdown(false), 
		m_queue(), 
		m_threads(MIN(n_threads, std::thread::hardware_concurrency())) {
		LOG(INFO) << "running thread pool with threads=" << n_threads << "; max possible threads on sys " << std::thread::hardware_concurrency();
		Init();
	}

	void ThreadPool::Init() {
		if (m_init) {
			return;
		}

		for (size_t i = 0; i < m_threads.size(); ++i) {
			m_threads[i] = std::thread(Worker(i, this));
		}

		m_init = true;
	}

	// Waits until threads finish their current task and shutdowns the pool
	void ThreadPool::shutdown() {
		if (!m_init) {
			return;
		}

		m_shutdown = true;
		m_cv.notify_all();

		for (size_t i = 0; i < m_threads.size(); ++i) {
			if (m_threads[i].joinable()) {
				m_threads[i].join();
			}
		}
	}

    ThreadPool::~ThreadPool() {
		shutdown();
	}
}
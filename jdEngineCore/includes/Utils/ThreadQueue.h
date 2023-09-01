#pragma once
#include <queue>
#include <shared_mutex>

namespace jd::utils
{
	template<typename T>
	class ThreadQueue final
	{
	public:
		ThreadQueue() = default;
		ThreadQueue(const ThreadQueue&) = delete;
		ThreadQueue& operator=(const ThreadQueue&) = delete;
		ThreadQueue(ThreadQueue&&) noexcept = default;
		ThreadQueue& operator=(ThreadQueue&&) noexcept = default;

		template<typename... Args, typename = std::enable_if_t<std::is_same_v<Args, T>>>
		void emplace(Args&&... args) {
			std::scoped_lock<std::mutex> lk(m_);
			(q_.push(std::forward<Args>(args)), ...);
		}

		void push(const T& t) {
			std::scoped_lock lk(m_);
			q_.push(t);
		}

		bool dequeue(T& t) {
			std::scoped_lock lk(m_);

			if (q_.empty()) {
				return false;
			}
			t = std::move(q_.front());
			q_.pop();

			return true;
		}

		void pop() {
			std::scoped_lock lk(m_);
			if (q_.empty()) {
				return;
			}
			q_.pop();
		}

		bool empty() const {
			std::shared_lock lk(m_);
			return q_.empty();
		}

		size_t size() const {
			std::shared_lock lk(m_);
			return q_.size();
		}

	private:
		std::queue<T> q_;
		mutable std::shared_mutex m_;
	};
}
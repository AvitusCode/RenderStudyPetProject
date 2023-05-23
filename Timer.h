#include <chrono>

class Timer
{
public:
	Timer();
	void start();
	void stop();
	float elapsed();
	float difference();
	size_t current();
private:
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_stop;
	std::chrono::steady_clock::time_point m_current;
};

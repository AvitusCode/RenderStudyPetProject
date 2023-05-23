#include "Timer.h"

Timer::Timer()
{
	start();
	stop();
}

using jd_clock = std::chrono::steady_clock;
using clock_freq = std::chrono::nanoseconds;

// Records current time in start variable
void Timer::start() {
	m_start = jd_clock::now();
}

// Records current time in stop variable
void Timer::stop() {
	m_stop = jd_clock::now();
}

// Get current time from previous Start call
float Timer::elapsed()
{
	m_current = jd_clock::now();
	return static_cast<float>(std::chrono::duration_cast<clock_freq>(m_current - m_start).count())
		/ static_cast<float>(std::chrono::duration_cast<clock_freq>(std::chrono::seconds(1)).count());

}

float Timer::difference() {
	return static_cast<float>(std::chrono::duration_cast<clock_freq>(m_stop - m_start).count());
}

size_t Timer::current()
{
	m_current = jd_clock::now();
	return std::chrono::duration_cast<clock_freq>(m_current.time_since_epoch()).count();
}

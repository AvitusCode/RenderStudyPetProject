#pragma once
#include <iostream>
#include <string>
#include <chrono>

class Profiler
{
public:
	Profiler(std::string msg = "") : message(std::move(msg)), start(std::chrono::steady_clock::now()) {};
	~Profiler() noexcept
	{
		auto end = std::chrono::steady_clock::now();
		std::cout << message << " sec: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000 << std::endl;
	}

private:
	std::string message;
	std::chrono::steady_clock::time_point start;
};

#define ID_IMPL(lineo) _uniq_loc_id##lineo
#define UNIQ_ID(lineo) ID_IMPL(lineo)

#define PROFILER(message) \
    Profiler UNIQ_ID(__LINE__){message}
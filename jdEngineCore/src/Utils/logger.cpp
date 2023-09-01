#include "ECS/configurations.h"
#include "Utils/logger.h"
#include "Utils/File.h"
#include "Utils/jd_string.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

namespace jd::logging
{
	const char* LoggerError::name() const noexcept {
		return "Logger errors";
	}

	std::string LoggerError::message(int err) const {
		switch (err)
		{
		case 0: return "[INFO]: ";
		case 1: return "[WARNING]: ";
		case 2: return "[ERROR]: ";
		case 3: return "[FATAL]: ";
		}

		return "unreachable!!!";
	}

	std::error_code make_error_code(LogErrors err) noexcept {
		return std::error_code(static_cast<int>(err), LoggerError::instance());
	}

	class DefaultFileLogger final
	{
	public:
		DefaultFileLogger();
		void toFile(std::string_view str);
		~DefaultFileLogger();

	private:
		std::ofstream file;
	};

	DefaultFileLogger::DefaultFileLogger()
	{
		jd::filesystem::createDirIfNotExists(ROOT_DIR "log");
		file.open(ROOT_DIR "log/jd_log.txt", std::ios::out | std::ios::trunc);
	}

	void DefaultFileLogger::toFile(std::string_view str) {
		file << str;
	}

	DefaultFileLogger::~DefaultFileLogger() {
		file << std::endl;
	}

	void DefaultLogStd(std::string_view str) {
		std::cerr << str;
	}

	void DefaultLogFile(std::string_view str) {
		static DefaultFileLogger fileLogger{};
		fileLogger.toFile(str);
	}

	void DefaultPrefixFunc(std::ostream& os, std::string_view info, int num)
	{
#ifdef _WIN32
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		os << jd::fmt::print("LOCAL TIME %02d/%02d/%02d %02d:%02d:%02d ", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);
#else
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		os << jd::fmt::print("LOCAL TIME %s ", asctime(timeinfo));
#endif
		os << jd::strings::getFileName(info) << ":" << num << " ";
	}
}
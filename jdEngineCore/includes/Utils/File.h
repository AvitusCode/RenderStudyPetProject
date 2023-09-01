#pragma once
#include <cstdint>
#include <string>
#include <optional>

namespace jd::filesystem
{
	struct FileTime
	{
		uint64_t creation;
		uint64_t lastWrite;
		uint64_t lastAccess;
	};

	std::optional<std::string> FileOpen();
	bool removeDir(const char* directoryName);
	bool isDir(const char* directoryName);
	bool isFile(const char* directoryName);
	bool moveFile(const char* from, const char* to);
	bool createDirIfNotExists(const char* directoryName);
	std::string getSpecialAppFolder();
	FileTime getFileTimes(const char* fileOrDirName);
}
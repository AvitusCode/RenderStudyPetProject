#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <sstream>


namespace jd::strings
{
	inline std::vector<std::string> split(std::string_view sv, char sep)
	{
		std::vector<std::string> result;

		while (true)
		{
			size_t pos = sv.find(sep);

			if (pos == sv.npos)
			{
				if (sv.size() > 0) {
					result.push_back(std::string(sv));
				}

				break;
			}

			std::string_view temp = sv.substr(0, pos);
			if (temp.size() != 0) {
				result.push_back(std::string(temp));
			}

			sv.remove_prefix(pos + 1);
		}

		return result;
	}


	inline std::string upDirectory(std::string_view sv, char slash)
	{
		bool isTrailingSlash = sv.back() == slash;
		std::vector<std::string> subPath = split(sv, slash);
		std::string result{};

		for (size_t i = 0; i < subPath.size() - 1; i++)
		{
			if (i > 0) {
				result += slash;
			}
			result += subPath[i];
		}

		if (isTrailingSlash && !result.empty()) {
			result += slash;
		}

		return result;
	}

	inline std::string makeCorrectSlashes(std::string_view path, char slash)
	{
		std::string result = std::string(path);

		for (char& c : result) {
			if (c == '\\' || c == '/') {
				c = slash;
			}
		}

		return result;
	}

	inline std::string getDirPath(std::string_view path)
	{
		size_t indx = path.find_last_of('\\');
		if (indx == path.npos) {
			indx = path.find_last_of('/');
		}

		if (indx == path.npos) {
			return "";
		}

		return std::string(path.substr(0, indx + 1));
	}

	inline std::string formatImpl(const char* str, std::stringstream& ss)
	{
		ss << str;
		return ss.str();
	}

	template<typename T, typename... Args>
	inline std::string formatImpl(const char* str, std::stringstream& ss, T arg, const Args&... args)
	{
		std::string result{};

		while (*str != 0)
		{
			if (*str == '{' && *(str + 1) == '}')
			{
				ss << arg;
				result = formatImpl(str + 2 * sizeof(char), ss, args...);
				break;
			}
			else {
				ss << *str;
			}

			str++;
		}

		return result;
	}

	template<typename... Args>
	inline std::string format(const std::string& fmt, const Args&... args)
	{
		std::stringstream ss;
		return formatImpl(fmt.c_str(), ss, args...);
	}
}
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace codestat
{
	struct FileCount
	{
		std::filesystem::path Path;
		std::string Extension;
		std::uintmax_t ByteCount = 0;
		std::uintmax_t TotalLines = 0;
		std::uintmax_t BlankLines = 0;
		std::uintmax_t LongestLine = 0;
	};
}

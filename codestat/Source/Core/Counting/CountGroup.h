#pragma once

#include <cstdint>
#include <string>

namespace codestat
{
	struct CountGroup
	{
		std::string Label;
		std::uintmax_t FileCount = 0;
		std::uintmax_t TotalLines = 0;
		std::uintmax_t BlankLines = 0;
		std::uintmax_t ByteCount = 0;
		std::uintmax_t LongestLine = 0;
	};
}

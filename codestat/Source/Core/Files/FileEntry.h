#pragma once

#include <cstdint>
#include <filesystem>

namespace codestat
{
	struct FileEntry
	{
		std::filesystem::path Path;
		std::uintmax_t ByteCount = 0;
	};
}

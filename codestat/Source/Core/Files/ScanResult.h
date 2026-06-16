#pragma once

#include <Source/Core/Files/FileEntry.h>

#include <string>
#include <vector>

namespace codestat
{
	struct ScanResult
	{
		std::vector<FileEntry> Files;
		std::vector<std::string> Errors;
	};
}

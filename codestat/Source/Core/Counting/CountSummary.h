#pragma once

#include <Source/Core/Counting/CountGroup.h>

#include <vector>

namespace codestat
{
	struct CountSummary
	{
		CountGroup Total;
		std::vector<CountGroup> ByExtension;
		std::vector<CountGroup> ByDirectory;
	};
}

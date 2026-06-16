#pragma once

#include <Source/App/CLI/CommandLineSettings.h>

#include <string>

namespace codestat
{
	struct CommandLineParseResult
	{
		CommandLineSettings Settings;
		bool Succeeded = true;
		bool ShowUsage = false;
		std::string Message;
	};
}

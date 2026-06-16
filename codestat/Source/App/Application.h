#pragma once

#include <Source/App/CLI/CommandLineSettings.h>
#include <Source/Core/Counting/CountGroup.h>
#include <Source/Core/Counting/CountSummary.h>

#include <string>
#include <vector>

namespace codestat
{
	class Application
	{
	public:
		Application() = default;
		~Application() = default;

		int Run(int argumentCount, char* arguments[]) const;

	private:
		std::string BuildDetailTitle(const CommandLineSettings& settings) const;
		std::vector<CountGroup> SelectDetailGroups(const CountSummary& summary, const CommandLineSettings& settings) const;
	};
}

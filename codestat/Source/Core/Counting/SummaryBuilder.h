#pragma once

#include <Source/Core/Counting/FileCount.h>
#include <Source/Core/Counting/CountSummary.h>

#include <filesystem>
#include <map>
#include <string>

namespace codestat
{
	class SummaryBuilder
	{
	public:
		explicit SummaryBuilder(const std::filesystem::path& rootPath);
		~SummaryBuilder() = default;

		void Add(const FileCount& count);
		CountSummary Build() const;

	private:
		std::filesystem::path RootPath;
		CountGroup Total;
		std::map<std::string, CountGroup> ExtensionGroups;
		std::map<std::string, CountGroup> DirectoryGroups;

		void AddToGroup(CountGroup& group, const FileCount& count) const;
		std::string BuildDirectoryLabel(const std::filesystem::path& path) const;
	};
}

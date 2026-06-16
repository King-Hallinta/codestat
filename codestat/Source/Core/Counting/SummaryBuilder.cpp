#include <Source/Core/Counting/SummaryBuilder.h>

#include <algorithm>
#include <system_error>

namespace codestat
{
	SummaryBuilder::SummaryBuilder(const std::filesystem::path& rootPath)
		: RootPath(rootPath)
	{
	}

	void SummaryBuilder::Add(const FileCount& count)
	{
		Total.Label = "total";
		AddToGroup(Total, count);

		CountGroup& extensionGroup = ExtensionGroups[count.Extension];
		extensionGroup.Label = count.Extension;
		AddToGroup(extensionGroup, count);

		std::string directory = BuildDirectoryLabel(count.Path.parent_path());

		CountGroup& directoryGroup = DirectoryGroups[directory];
		directoryGroup.Label = directory;
		AddToGroup(directoryGroup, count);
	}

	CountSummary SummaryBuilder::Build() const
	{
		CountSummary summary;
		summary.Total = Total;

		for (const auto& pair : ExtensionGroups)
		{
			summary.ByExtension.push_back(pair.second);
		}

		for (const auto& pair : DirectoryGroups)
		{
			summary.ByDirectory.push_back(pair.second);
		}

		return summary;
	}

	void SummaryBuilder::AddToGroup(CountGroup& group, const FileCount& count) const
	{
		++group.FileCount;
		group.TotalLines += count.TotalLines;
		group.BlankLines += count.BlankLines;
		group.ByteCount += count.ByteCount;
		group.LongestLine = std::max(group.LongestLine, count.LongestLine);
	}

	std::string SummaryBuilder::BuildDirectoryLabel(const std::filesystem::path& path) const
	{
		std::error_code error;
		std::filesystem::path relativePath = std::filesystem::relative(path, RootPath, error);

		if (error)
		{
			std::string value = path.generic_string();

			if (value.empty())
			{
				return ".";
			}

			return value;
		}

		std::string value = relativePath.generic_string();

		if (value.empty())
		{
			return ".";
		}

		return value;
	}
}

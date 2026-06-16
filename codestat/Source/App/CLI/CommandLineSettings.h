#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace codestat
{
	enum class GroupMode
	{
		Total,
		Extension,
		Directory
	};

	enum class SortField
	{
		Name,
		Files,
		Lines,
		Blank,
		Bytes,
		Longest
	};

	struct CommandLineSettings
	{
		std::optional<std::filesystem::path> RootPath;
		std::vector<std::string> IncludePatterns;
		std::vector<std::string> ExcludePatterns;
		std::vector<std::string> Extensions;
		std::optional<std::string> GroupFilter;
		std::optional<std::uintmax_t> MaxBytes;
		std::optional<std::size_t> TopCount;
		GroupMode GroupBy = GroupMode::Extension;
		SortField SortBy = SortField::Lines;
		bool IncludeHidden = false;
		bool SortAscending = false;
		bool ShowHelp = false;
	};
}

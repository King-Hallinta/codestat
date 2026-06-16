#pragma once

#include <Source/App/CLI/CommandLineSettings.h>
#include <Source/Core/Files/FileEntry.h>
#include <Source/Core/Filters/GlobPattern.h>

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace codestat
{
	class FileFilter
	{
	public:
		explicit FileFilter(const CommandLineSettings& settings);
		~FileFilter() = default;

		bool AllowsDirectory(const std::filesystem::path& path) const;
		bool AllowsFile(const FileEntry& entry) const;

	private:
		std::vector<GlobPattern> IncludePatterns;
		std::vector<GlobPattern> ExcludePatterns;
		std::vector<std::string> Extensions;
		std::optional<std::uintmax_t> MaxBytes;
		bool IncludeHidden = false;

		bool MatchesAnyPattern(const std::vector<GlobPattern>& patterns, const std::string& pathText) const;
		bool HasHiddenSegment(const std::filesystem::path& path) const;
		bool AllowsExtension(const std::filesystem::path& path) const;
		std::string NormalizePath(const std::filesystem::path& path) const;
		std::string NormalizeExtension(std::string value) const;
	};
}

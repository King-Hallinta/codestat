#include <Source/Core/Filters/FileFilter.h>

#include <algorithm>
#include <cctype>

namespace codestat
{
	FileFilter::FileFilter(const CommandLineSettings& settings)
		: Extensions(settings.Extensions),
		MaxBytes(settings.MaxBytes),
		IncludeHidden(settings.IncludeHidden)
	{
		for (const std::string& pattern : settings.IncludePatterns)
		{
			IncludePatterns.emplace_back(pattern);
		}

		for (const std::string& pattern : settings.ExcludePatterns)
		{
			ExcludePatterns.emplace_back(pattern);
		}
	}

	bool FileFilter::AllowsDirectory(const std::filesystem::path& path) const
	{
		if (IncludeHidden == false and HasHiddenSegment(path))
		{
			return false;
		}

		return true;
	}

	bool FileFilter::AllowsFile(const FileEntry& entry) const
	{
		const std::string pathText = NormalizePath(entry.Path);

		if (IncludeHidden == false and HasHiddenSegment(entry.Path))
		{
			return false;
		}

		if (MaxBytes.has_value() and entry.ByteCount > MaxBytes.value())
		{
			return false;
		}

		if (AllowsExtension(entry.Path) == false)
		{
			return false;
		}

		if (IncludePatterns.empty() == false and MatchesAnyPattern(IncludePatterns, pathText) == false)
		{
			return false;
		}

		if (MatchesAnyPattern(ExcludePatterns, pathText))
		{
			return false;
		}

		return true;
	}

	bool FileFilter::MatchesAnyPattern(const std::vector<GlobPattern>& patterns, const std::string& pathText) const
	{
		for (const GlobPattern& pattern : patterns)
		{
			if (pattern.Matches(pathText))
			{
				return true;
			}
		}

		return false;
	}

	bool FileFilter::HasHiddenSegment(const std::filesystem::path& path) const
	{
		for (const std::filesystem::path& segment : path)
		{
			const std::string text = segment.filename().string();

			if (text.size() > 1 and text[0] == '.')
			{
				return true;
			}
		}

		return false;
	}

	bool FileFilter::AllowsExtension(const std::filesystem::path& path) const
	{
		if (Extensions.empty())
		{
			return true;
		}

		std::string extension = NormalizeExtension(path.extension().string());

		return std::find(Extensions.begin(), Extensions.end(), extension) not_eq Extensions.end();
	}

	std::string FileFilter::NormalizePath(const std::filesystem::path& path) const
	{
		std::string value = path.generic_string();
		std::transform(value.begin(), value.end(), value.begin(),
			[](unsigned char character)
			{
				return static_cast<char>(std::tolower(character));
			});

		return value;
	}

	std::string FileFilter::NormalizeExtension(std::string value) const
	{
		if (value.empty() == false and value.front() == '.')
		{
			value.erase(value.begin());
		}

		std::transform(value.begin(), value.end(), value.begin(),
			[](unsigned char character)
			{
				return static_cast<char>(std::tolower(character));
			});

		return value;
	}
}

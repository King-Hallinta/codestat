#include <Source/Core/Counting/LineCounter.h>

#include <algorithm>
#include <cctype>

namespace codestat
{
	FileCount LineCounter::Count(const std::filesystem::path& path, std::uintmax_t byteCount, const std::string& content) const
	{
		FileCount count;
		count.Path = path;
		count.Extension = NormalizeExtension(path.extension().string());
		count.ByteCount = byteCount;

		std::size_t lineBegin = 0;

		for (std::size_t index = 0; index < content.size(); ++index)
		{
			if (content[index] == '\n')
			{
				std::size_t lineEnd = index;

				if (lineEnd > lineBegin and content[lineEnd - 1] == '\r')
				{
					--lineEnd;
				}

				++count.TotalLines;
				count.LongestLine = std::max<std::uintmax_t>(count.LongestLine, lineEnd - lineBegin);

				if (IsBlankLine(content, lineBegin, lineEnd))
				{
					++count.BlankLines;
				}

				lineBegin = index + 1;
			}
		}

		if (lineBegin < content.size())
		{
			std::size_t lineEnd = content.size();

			if (lineEnd > lineBegin and content[lineEnd - 1] == '\r')
			{
				--lineEnd;
			}

			++count.TotalLines;
			count.LongestLine = std::max<std::uintmax_t>(count.LongestLine, lineEnd - lineBegin);

			if (IsBlankLine(content, lineBegin, lineEnd))
			{
				++count.BlankLines;
			}
		}

		return count;
	}

	bool LineCounter::IsBlankLine(const std::string& content, std::size_t begin, std::size_t end) const
	{
		for (std::size_t index = begin; index < end; ++index)
		{
			if (std::isspace(static_cast<unsigned char>(content[index])) == false)
			{
				return false;
			}
		}

		return true;
	}

	std::string LineCounter::NormalizeExtension(std::string value) const
	{
		if (value.empty() == false and value.front() == '.')
		{
			value.erase(value.begin());
		}

		if (value.empty())
		{
			value = "[none]";
		}

		std::transform(value.begin(), value.end(), value.begin(),
			[](unsigned char character)
			{
				return static_cast<char>(std::tolower(character));
			});

		return value;
	}
}

#include <Source/Core/Filters/GlobPattern.h>

#include <cctype>
#include <utility>

namespace codestat
{
	GlobPattern::GlobPattern(std::string pattern)
		: Pattern(std::move(pattern))
	{
	}

	bool GlobPattern::Matches(const std::string& value) const
	{
		return MatchesAt(0, 0, value);
	}

	bool GlobPattern::MatchesAt(std::size_t patternIndex, std::size_t valueIndex, const std::string& value) const
	{
		std::size_t currentPattern = patternIndex;
		std::size_t currentValue = valueIndex;
		std::size_t starPattern = std::string::npos;
		std::size_t starValue = std::string::npos;

		while (currentValue < value.size())
		{
			if (currentPattern < Pattern.size() and
				(Pattern[currentPattern] == '?' or NormalizeCharacter(Pattern[currentPattern]) == NormalizeCharacter(value[currentValue])))
			{
				++currentPattern;
				++currentValue;
			}
			else if (currentPattern < Pattern.size() and Pattern[currentPattern] == '*')
			{
				starPattern = currentPattern;
				starValue = currentValue;
				++currentPattern;
			}
			else if (starPattern not_eq std::string::npos)
			{
				currentPattern = starPattern + 1;
				++starValue;
				currentValue = starValue;
			}
			else
			{
				return false;
			}
		}

		while (currentPattern < Pattern.size() and Pattern[currentPattern] == '*')
		{
			++currentPattern;
		}

		return currentPattern == Pattern.size();
	}

	char GlobPattern::NormalizeCharacter(char character) const
	{
		return static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
	}
}

#pragma once

#include <string>

namespace codestat
{
	class GlobPattern
	{
	public:
		explicit GlobPattern(std::string pattern);
		~GlobPattern() = default;

		bool Matches(const std::string& value) const;

	private:
		std::string Pattern;

		bool MatchesAt(std::size_t patternIndex, std::size_t valueIndex, const std::string& value) const;
		char NormalizeCharacter(char character) const;
	};
}

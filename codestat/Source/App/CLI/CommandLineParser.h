#pragma once

#include <Source/App/CLI/CommandLineParseResult.h>

#include <cstdint>
#include <string>

namespace codestat
{
	class CommandLineParser
	{
	public:
		CommandLineParser() = default;
		~CommandLineParser() = default;

		CommandLineParseResult Parse(int argumentCount, char* arguments[]) const;
		std::string BuildHelpText() const;

	private:
		bool ReadValue(int& index, int argumentCount, char* arguments[], std::string& value, std::string& message) const;
		bool TryParseGroupMode(const std::string& value, GroupMode& result) const;
		bool TryParseSortField(const std::string& value, SortField& result) const;
		std::string NormalizeExtension(std::string value) const;
		std::string NormalizeText(std::string value) const;
		bool TryParseByteCount(const std::string& value, std::uintmax_t& result) const;
		bool TryParseTopCount(const std::string& value, std::size_t& result) const;
	};
}

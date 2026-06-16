#include <Source/Core/Text/TextReader.h>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <utility>

namespace codestat
{
	TextContent TextReader::Read(const std::filesystem::path& path) const
	{
		std::ifstream input(path, std::ios::binary);

		if (input.is_open() == false)
		{
			return {};
		}

		std::string value{
			std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>() };

		if (input.bad())
		{
			return {};
		}

		if (ContainsNullByte(value))
		{
			return {};
		}

		TextContent content;
		content.Accepted = true;
		content.Value = std::move(value);

		return content;
	}

	bool TextReader::ContainsNullByte(const std::string& value) const
	{
		return std::find(value.begin(), value.end(), '\0') not_eq value.end();
	}
}

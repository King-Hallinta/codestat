#pragma once

#include <Source/Core/Text/TextContent.h>

#include <filesystem>

namespace codestat
{
	class TextReader
	{
	public:
		TextReader() = default;
		~TextReader() = default;

		TextContent Read(const std::filesystem::path& path) const;

	private:
		bool ContainsNullByte(const std::string& value) const;
	};
}

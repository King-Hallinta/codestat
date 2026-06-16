#pragma once

#include <Source/Core/Counting/FileCount.h>

#include <filesystem>
#include <string>

namespace codestat
{
	class LineCounter
	{
	public:
		LineCounter() = default;
		~LineCounter() = default;

		FileCount Count(const std::filesystem::path& path, std::uintmax_t byteCount, const std::string& content) const;

	private:
		bool IsBlankLine(const std::string& content, std::size_t begin, std::size_t end) const;
		std::string NormalizeExtension(std::string value) const;
	};
}

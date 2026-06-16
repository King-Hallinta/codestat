#pragma once

#include <Source/Core/Files/ScanResult.h>

#include <filesystem>

namespace codestat
{
	class FileFilter;

	class FileScanner
	{
	public:
		FileScanner() = default;
		~FileScanner() = default;

		ScanResult Scan(const std::filesystem::path& rootPath, const FileFilter& filter) const;

	private:
		void AddFile(const std::filesystem::path& path, const FileFilter& filter, ScanResult& result) const;
	};
}

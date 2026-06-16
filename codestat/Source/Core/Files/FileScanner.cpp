#include <Source/Core/Files/FileScanner.h>

#include <Source/Core/Filters/FileFilter.h>

#include <system_error>

namespace codestat
{
	ScanResult FileScanner::Scan(const std::filesystem::path& rootPath, const FileFilter& filter) const
	{
		ScanResult result;
		std::error_code error;

		if (std::filesystem::exists(rootPath, error) == false)
		{
			result.Errors.push_back(rootPath.string() + ": path does not exist.");

			return result;
		}

		if (std::filesystem::is_regular_file(rootPath, error))
		{
			AddFile(rootPath, filter, result);

			return result;
		}

		if (std::filesystem::is_directory(rootPath, error) == false)
		{
			result.Errors.push_back(rootPath.string() + ": path is neither a file nor a directory.");

			return result;
		}

		std::filesystem::recursive_directory_iterator iterator(
			rootPath,
			std::filesystem::directory_options::skip_permission_denied,
			error);
		std::filesystem::recursive_directory_iterator end;

		if (error)
		{
			result.Errors.push_back(rootPath.string() + ": " + error.message());

			return result;
		}

		while (iterator not_eq end)
		{
			const std::filesystem::path currentPath = iterator->path();
			error.clear();
			bool directory = iterator->is_directory(error);

			if (error)
			{
				result.Errors.push_back(currentPath.string() + ": " + error.message());
				error.clear();
			}
			else if (directory)
			{
				if (filter.AllowsDirectory(currentPath) == false)
				{
					iterator.disable_recursion_pending();
				}
			}
			else
			{
				bool regularFile = iterator->is_regular_file(error);

				if (error)
				{
					result.Errors.push_back(currentPath.string() + ": " + error.message());
					error.clear();
				}
				else if (regularFile)
				{
					AddFile(currentPath, filter, result);
				}
			}

			iterator.increment(error);

			if (error)
			{
				result.Errors.push_back(currentPath.string() + ": " + error.message());
				error.clear();
			}
		}

		return result;
	}

	void FileScanner::AddFile(const std::filesystem::path& path, const FileFilter& filter, ScanResult& result) const
	{
		std::error_code error;
		std::uintmax_t byteCount = std::filesystem::file_size(path, error);

		if (error)
		{
			result.Errors.push_back(path.string() + ": " + error.message());

			return;
		}

		FileEntry entry;
		entry.Path = path;
		entry.ByteCount = byteCount;

		if (filter.AllowsFile(entry))
		{
			result.Files.push_back(entry);
		}
	}
}

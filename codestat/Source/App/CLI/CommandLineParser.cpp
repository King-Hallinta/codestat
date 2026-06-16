#include <Source/App/CLI/CommandLineParser.h>

#include <algorithm>
#include <charconv>
#include <cctype>
#include <cstdint>
#include <sstream>
#include <system_error>

namespace codestat
{
	CommandLineParseResult CommandLineParser::Parse(int argumentCount, char* arguments[]) const
	{
		CommandLineParseResult result;
		bool rootPathSet = false;

		if (argumentCount <= 1)
		{
			result.ShowUsage = true;
			result.Settings.ShowHelp = true;

			return result;
		}

		for (int index = 1; index < argumentCount; ++index)
		{
			std::string argument = arguments[index];

			if (argument == "--help" or argument == "-h")
			{
				result.Settings.ShowHelp = true;
			}
			else if (argument == "--include")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				result.Settings.IncludePatterns.push_back(value);
			}
			else if (argument == "--exclude")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				result.Settings.ExcludePatterns.push_back(value);
			}
			else if (argument == "--extension")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				result.Settings.Extensions.push_back(NormalizeExtension(value));
			}
			else if (argument == "--max-bytes")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				std::uintmax_t byteCount = 0;

				if (TryParseByteCount(value, byteCount) == false)
				{
					result.Succeeded = false;
					result.Message = "--max-bytes requires a non-negative integer value.";

					return result;
				}

				result.Settings.MaxBytes = byteCount;
			}
			else if (argument == "--hidden")
			{
				result.Settings.IncludeHidden = true;
			}
			else if (argument == "--here")
			{
				if (rootPathSet)
				{
					result.Succeeded = false;
					result.Message = "Only one root path can be provided.";

					return result;
				}

				std::error_code error;
				std::filesystem::path currentPath = std::filesystem::current_path(error);

				if (error)
				{
					result.Succeeded = false;
					result.Message = "Unable to resolve the current directory.";

					return result;
				}

				result.Settings.RootPath = currentPath;
				rootPathSet = true;
			}
			else if (argument == "--group" or argument == "--by")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				GroupMode groupMode = GroupMode::Extension;

				if (TryParseGroupMode(value, groupMode) == false)
				{
					result.Succeeded = false;
					result.Message = "--group requires one of: total, extension, directory.";

					return result;
				}

				result.Settings.GroupBy = groupMode;
			}
			else if (argument == "--filter")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				result.Settings.GroupFilter = NormalizeText(value);
			}
			else if (argument == "--sort")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				SortField sortField = SortField::Lines;

				if (TryParseSortField(value, sortField) == false)
				{
					result.Succeeded = false;
					result.Message = "--sort requires one of: name, files, lines, blank, bytes, longest.";

					return result;
				}

				result.Settings.SortBy = sortField;
			}
			else if (argument == "--top")
			{
				std::string value;

				if (ReadValue(index, argumentCount, arguments, value, result.Message) == false)
				{
					result.Succeeded = false;

					return result;
				}

				std::size_t topCount = 0;

				if (TryParseTopCount(value, topCount) == false)
				{
					result.Succeeded = false;
					result.Message = "--top requires a positive integer value.";

					return result;
				}

				result.Settings.TopCount = topCount;
			}
			else if (argument == "--ascending")
			{
				result.Settings.SortAscending = true;
			}
			else if (argument.size() > 2 and argument[0] == '-' and argument[1] == '-')
			{
				result.Succeeded = false;
				result.Message = "Unknown option: " + argument;

				return result;
			}
			else if (rootPathSet == false)
			{
				result.Settings.RootPath = argument;
				rootPathSet = true;
			}
			else
			{
				result.Succeeded = false;
				result.Message = "Only one root path can be provided.";

				return result;
			}
		}

		if (result.Settings.ShowHelp == false and rootPathSet == false)
		{
			result.Succeeded = false;
			result.ShowUsage = true;
			result.Message = "A path is required. Use --here to scan the current directory.";
		}

		return result;
	}

	std::string CommandLineParser::BuildHelpText() const
	{
		std::ostringstream output;
		output << "Usage: codestat <path> [options]\n";
		output << "       codestat --here [options]\n";
		output << "\n";
		output << "Options:\n";
		output << "  --group <mode>        Group modes: total, extension, directory.\n";
		output << "  --by <mode>           Alias for --group with the same modes.\n";
		output << "  --filter <text>       Filter the selected group labels.\n";
		output << "  --sort <field>        Sort by name, files, lines, blank, bytes, or longest.\n";
		output << "  --top <number>        Show only the top matching groups.\n";
		output << "  --ascending           Sort in ascending order.\n";
		output << "  --here                Scan the current working directory.\n";
		output << "  --include <glob>      Include paths matching a wildcard pattern.\n";
		output << "  --exclude <glob>      Exclude paths matching a wildcard pattern.\n";
		output << "  --extension <ext>     Include only files with an extension.\n";
		output << "  --max-bytes <number>  Skip files larger than the byte limit.\n";
		output << "  --hidden              Include dot-prefixed files and directories.\n";
		output << "  --help                Show this help text.\n";

		return output.str();
	}

	bool CommandLineParser::ReadValue(int& index, int argumentCount, char* arguments[], std::string& value, std::string& message) const
	{
		if (index + 1 >= argumentCount)
		{
			message = std::string(arguments[index]) + " requires a value.";

			return false;
		}

		++index;
		value = arguments[index];

		return true;
	}

	bool CommandLineParser::TryParseGroupMode(const std::string& value, GroupMode& result) const
	{
		const std::string normalized = NormalizeText(value);

		if (normalized == "total")
		{
			result = GroupMode::Total;

			return true;
		}

		if (normalized == "extension")
		{
			result = GroupMode::Extension;

			return true;
		}

		if (normalized == "directory")
		{
			result = GroupMode::Directory;

			return true;
		}

		return false;
	}

	bool CommandLineParser::TryParseSortField(const std::string& value, SortField& result) const
	{
		const std::string normalized = NormalizeText(value);

		if (normalized == "name")
		{
			result = SortField::Name;

			return true;
		}

		if (normalized == "files")
		{
			result = SortField::Files;

			return true;
		}

		if (normalized == "lines")
		{
			result = SortField::Lines;

			return true;
		}

		if (normalized == "blank")
		{
			result = SortField::Blank;

			return true;
		}

		if (normalized == "bytes")
		{
			result = SortField::Bytes;

			return true;
		}

		if (normalized == "longest")
		{
			result = SortField::Longest;

			return true;
		}

		return false;
	}

	std::string CommandLineParser::NormalizeExtension(std::string value) const
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

	std::string CommandLineParser::NormalizeText(std::string value) const
	{
		std::transform(value.begin(), value.end(), value.begin(),
			[](unsigned char character)
			{
				return static_cast<char>(std::tolower(character));
			});

		return value;
	}

	bool CommandLineParser::TryParseByteCount(const std::string& value, std::uintmax_t& result) const
	{
		const char* begin = value.data();
		const char* end = begin + value.size();
		std::from_chars_result parseResult = std::from_chars(begin, end, result);

		return parseResult.ec == std::errc() and parseResult.ptr == end;
	}

	bool CommandLineParser::TryParseTopCount(const std::string& value, std::size_t& result) const
	{
		std::uintmax_t parsedValue = 0;

		if (TryParseByteCount(value, parsedValue) == false or parsedValue == 0)
		{
			return false;
		}

		result = static_cast<std::size_t>(parsedValue);

		return true;
	}
}

#include <Source/App/Application.h>

#include <Source/App/CLI/CommandLineParser.h>
#include <Source/Core/Counting/LineCounter.h>
#include <Source/Core/Counting/SummaryBuilder.h>
#include <Source/Core/Files/FileScanner.h>
#include <Source/Core/Filters/FileFilter.h>
#include <Source/Core/Text/TextReader.h>
#include <Source/Reporting/Table/TableFormatter.h>
#include <Source/Support/Errors/ExitCode.h>

#include <iostream>
#include <algorithm>
#include <cctype>

namespace codestat
{
	int Application::Run(int argumentCount, char* arguments[]) const
	{
		CommandLineParser parser;
		CommandLineParseResult parseResult = parser.Parse(argumentCount, arguments);

		if (parseResult.Succeeded == false)
		{
			std::cerr << parseResult.Message << '\n';

			if (parseResult.ShowUsage)
			{
				std::cerr << parser.BuildHelpText();
			}

			return static_cast<int>(ExitCode::UsageError);
		}

		if (parseResult.Settings.ShowHelp)
		{
			std::cout << parser.BuildHelpText();

			return static_cast<int>(ExitCode::Success);
		}

		FileFilter filter(parseResult.Settings);
		FileScanner scanner;
		TextReader textReader;
		LineCounter lineCounter;
		SummaryBuilder summaryBuilder(parseResult.Settings.RootPath.value());

		ScanResult scanResult = scanner.Scan(parseResult.Settings.RootPath.value(), filter);

		for (const FileEntry& entry : scanResult.Files)
		{
			TextContent content = textReader.Read(entry.Path);

			if (content.Accepted == false)
			{
				continue;
			}

			FileCount count = lineCounter.Count(entry.Path, entry.ByteCount, content.Value);
			summaryBuilder.Add(count);
		}

		CountSummary summary = summaryBuilder.Build();
		std::vector<CountGroup> detailGroups = SelectDetailGroups(summary, parseResult.Settings);
		std::string detailTitle = BuildDetailTitle(parseResult.Settings);

		TableFormatter formatter;
		std::cout << formatter.Render(summary.Total, detailTitle, detailGroups);

		if (scanResult.Errors.empty() == false)
		{
			std::cerr << "\nSkipped paths:\n";

			for (const std::string& error : scanResult.Errors)
			{
				std::cerr << "  " << error << '\n';
			}
		}

		return static_cast<int>(ExitCode::Success);
	}

	std::string Application::BuildDetailTitle(const CommandLineSettings& settings) const
	{
		if (settings.GroupBy == GroupMode::Total)
		{
			return {};
		}

		if (settings.GroupBy == GroupMode::Extension)
		{
			return "By extension";
		}

		return "By directory";
	}

	std::vector<CountGroup> Application::SelectDetailGroups(const CountSummary& summary, const CommandLineSettings& settings) const
	{
		std::vector<CountGroup> groups;

		if (settings.GroupBy == GroupMode::Extension)
		{
			groups = summary.ByExtension;
		}
		else if (settings.GroupBy == GroupMode::Directory)
		{
			groups = summary.ByDirectory;
		}
		else
		{
			return groups;
		}

		if (settings.GroupFilter.has_value())
		{
			const std::string filterText = settings.GroupFilter.value();

			groups.erase(
				std::remove_if(
					groups.begin(),
					groups.end(),
					[&filterText](const CountGroup& group)
					{
						std::string label = group.Label;
						std::transform(label.begin(), label.end(), label.begin(),
							[](unsigned char character)
							{
								return static_cast<char>(std::tolower(character));
							});

						return label.find(filterText) == std::string::npos;
					}),
				groups.end());
		}

		std::sort(
			groups.begin(),
			groups.end(),
			[&settings](const CountGroup& left, const CountGroup& right)
			{
				auto compareValues =
					[&settings](const auto& first, const auto& second)
					{
						if (first == second)
						{
							return false;
						}

						if (settings.SortAscending)
						{
							return first < second;
						}

						return first > second;
					};

				switch (settings.SortBy)
				{
				case SortField::Name:
				{
					return compareValues(left.Label, right.Label);
				}

				case SortField::Files:
				{
					return compareValues(left.FileCount, right.FileCount);
				}

				case SortField::Blank:
				{
					return compareValues(left.BlankLines, right.BlankLines);
				}

				case SortField::Bytes:
				{
					return compareValues(left.ByteCount, right.ByteCount);
				}

				case SortField::Longest:
				{
					return compareValues(left.LongestLine, right.LongestLine);
				}

				case SortField::Lines:
				default:
				{
					return compareValues(left.TotalLines, right.TotalLines);
				}
				}
			});

		if (settings.TopCount.has_value() and groups.size() > settings.TopCount.value())
		{
			groups.resize(settings.TopCount.value());
		}

		return groups;
	}
}

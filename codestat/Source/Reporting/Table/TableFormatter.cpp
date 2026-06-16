#include <Source/Reporting/Table/TableFormatter.h>

#include <iomanip>
#include <sstream>

namespace
{
	constexpr int LabelWidth = 32;
	constexpr int RatioWidth = 9;
	constexpr int FileWidth = 10;
	constexpr int LineWidth = 12;
	constexpr int ByteWidth = 14;
	constexpr int LongestWidth = 14;

	constexpr const char* ResetColor = "\x1b[0m";
	constexpr const char* BorderColor = "\x1b[90m";
	constexpr const char* HeaderColor = "\x1b[1;36m";
	constexpr const char* TitleColor = "\x1b[1;37m";
	constexpr const char* SummaryColor = "\x1b[1;32m";
}

namespace codestat
{
	std::string TableFormatter::Render(const CountGroup& total, const std::string& detailTitle, const std::vector<CountGroup>& detailGroups) const
	{
		std::ostringstream output;
		WriteSection(output, "Summary", std::vector<CountGroup>{ total }, true);

		if (detailTitle.empty() == false)
		{
			WriteSection(output, detailTitle, detailGroups, false);
		}

		return output.str();
	}

	void TableFormatter::WriteSection(std::ostream& output, const std::string& title, const std::vector<CountGroup>& groups, bool emphasizeRows) const
	{
		if (title.empty() == false and output.tellp() not_eq std::streampos(0))
		{
			output << "\n";
		}

		output << TitleColor << title << ResetColor << "\n";
		WriteBorder(output);
		WriteHeader(output);
		WriteBorder(output);
		WriteRows(output, groups, emphasizeRows);
		WriteBorder(output);
	}

	void TableFormatter::WriteRows(std::ostream& output, const std::vector<CountGroup>& groups, bool emphasizeRows) const
	{
		if (groups.empty())
		{
			CountGroup emptyGroup;
			emptyGroup.Label = "(none)";
			WriteRow(output, emptyGroup, false);

			return;
		}

		for (const CountGroup& group : groups)
		{
			WriteRow(output, group, emphasizeRows);
		}
	}

	void TableFormatter::WriteHeader(std::ostream& output) const
	{
		output << HeaderColor;
		output << "| " << std::left << std::setw(LabelWidth) << "group";
		output << " | " << std::right << std::setw(LineWidth) << "blank";
		output << " | " << std::right << std::setw(RatioWidth) << "empty %";
		output << " | " << std::right << std::setw(ByteWidth) << "bytes";
		output << " | " << std::right << std::setw(LongestWidth) << "longest";
		output << " | " << std::right << std::setw(FileWidth) << "files";
		output << " | " << std::right << std::setw(LineWidth) << "lines" << " |";
		output << ResetColor << "\n";
	}

	void TableFormatter::WriteBorder(std::ostream& output) const
	{
		output << BorderColor;
		output << "+-" << std::string(LabelWidth, '-');
		output << "-+-" << std::string(LineWidth, '-');
		output << "-+-" << std::string(RatioWidth, '-');
		output << "-+-" << std::string(ByteWidth, '-');
		output << "-+-" << std::string(LongestWidth, '-');
		output << "-+-" << std::string(FileWidth, '-');
		output << "-+-" << std::string(LineWidth, '-') << "-+";
		output << ResetColor << "\n";
	}

	void TableFormatter::WriteRow(std::ostream& output, const CountGroup& group, bool emphasizeRow) const
	{
		std::string label = group.Label;

		if (label.size() > static_cast<std::size_t>(LabelWidth))
		{
			label = label.substr(0, LabelWidth - 3) + "...";
		}

		if (emphasizeRow)
		{
			output << SummaryColor;
		}

		output << "| " << std::left << std::setw(LabelWidth) << label;
		output << " | " << std::right << std::setw(LineWidth) << group.BlankLines;
		output << " | " << std::right << std::setw(RatioWidth) << FormatEmptyLineRatio(group);
		output << " | " << std::right << std::setw(ByteWidth) << group.ByteCount;
		output << " | " << std::right << std::setw(LongestWidth) << group.LongestLine;
		output << " | " << std::right << std::setw(FileWidth) << group.FileCount;
		output << " | " << std::right << std::setw(LineWidth) << group.TotalLines << " |";

		if (emphasizeRow)
		{
			output << ResetColor;
		}

		output << "\n";
	}

	std::string TableFormatter::FormatEmptyLineRatio(const CountGroup& group) const
	{
		if (group.TotalLines == 0)
		{
			return "0.0%";
		}

		const double ratio = static_cast<double>(group.BlankLines) * 100.0 / static_cast<double>(group.TotalLines);
		std::ostringstream output;
		output << std::fixed << std::setprecision(1) << ratio << "%";

		return output.str();
	}
}

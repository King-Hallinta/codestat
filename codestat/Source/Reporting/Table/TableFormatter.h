#pragma once

#include <Source/Core/Counting/CountSummary.h>

#include <ostream>
#include <string>
#include <vector>

namespace codestat
{
	class TableFormatter
	{
	public:
		TableFormatter() = default;
		~TableFormatter() = default;

		std::string Render(const CountGroup& total, const std::string& detailTitle, const std::vector<CountGroup>& detailGroups) const;

	private:
		void WriteSection(std::ostream& output, const std::string& title, const std::vector<CountGroup>& groups, bool emphasizeRows) const;
		void WriteRows(std::ostream& output, const std::vector<CountGroup>& groups, bool emphasizeRows) const;
		void WriteHeader(std::ostream& output) const;
		void WriteBorder(std::ostream& output) const;
		void WriteRow(std::ostream& output, const CountGroup& group, bool emphasizeRow) const;
		std::string FormatEmptyLineRatio(const CountGroup& group) const;
	};
}

set(CODESTAT_SOURCES
  "Source/App/Main.cpp"
  "Source/App/Application.cpp"
  "Source/App/Application.h"
  "Source/App/CLI/CommandLineParser.cpp"
  "Source/App/CLI/CommandLineParser.h"
  "Source/App/CLI/CommandLineParseResult.h"
  "Source/App/CLI/CommandLineSettings.h"
  "Source/Core/Files/FileEntry.h"
  "Source/Core/Files/FileScanner.cpp"
  "Source/Core/Files/FileScanner.h"
  "Source/Core/Files/ScanResult.h"
  "Source/Core/Filters/FileFilter.cpp"
  "Source/Core/Filters/FileFilter.h"
  "Source/Core/Filters/GlobPattern.cpp"
  "Source/Core/Filters/GlobPattern.h"
  "Source/Core/Counting/CountGroup.h"
  "Source/Core/Counting/CountSummary.h"
  "Source/Core/Counting/FileCount.h"
  "Source/Core/Counting/LineCounter.cpp"
  "Source/Core/Counting/LineCounter.h"
  "Source/Core/Counting/SummaryBuilder.cpp"
  "Source/Core/Counting/SummaryBuilder.h"
  "Source/Core/Text/TextContent.h"
  "Source/Core/Text/TextReader.cpp"
  "Source/Core/Text/TextReader.h"
  "Source/Reporting/Table/TableFormatter.cpp"
  "Source/Reporting/Table/TableFormatter.h"
  "Source/Support/Errors/ExitCode.h"
)

add_executable(codestat ${CODESTAT_SOURCES})

target_include_directories(codestat PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET codestat PROPERTY CXX_STANDARD 20)
endif()

# codestat

codestat is a command-line tool for counting basic file and line statistics in a file or directory tree. It is not syntax-aware: it does not count code lines, comment lines, or languages.

It is very similar to cloc (by @AlDanial). I started this project before I knew cloc existed.

codestat reads files that look like text, skips binary-looking files (simple NUL-byte check), and can summarize results as a total, by file extension, or by directory.

## Usage

Scan a path:

```
codestat <path>
```

## Options

```text
--here                  Scan the current directory
--group, --by <mode>    Show results by total, extension, or directory
--filter <text>         Filter displayed group labels
--sort <field>          Sort by name, files, lines, blank, bytes, or longest
--top <number>          Limit grouped output
--ascending             Sort ascending

--include <glob>        Include files matching a glob
--exclude <glob>        Exclude files matching a glob
--extension <ext>       Include only files with the given extension
--max-bytes <number>    Skip files larger than the given size
--hidden                Include hidden files and directories
--help                  Print usage
```

## Output

codestat prints a summary table and, depending on --group, a grouped table

Files are grouped by extension when extension grouping is used. Files with no extension are grouped under `[none]`. When grouped by directory, directory labels are shown relative to the selected root.

## Requirements

* C++20
* MSVC
* CMake
* Ninja

## Building

Configure and build the debug preset:

```
cmake --preset x64-debug
cmake --build out/build/x64-debug
```

Configure and build the release preset:

```
cmake --preset x64-release
cmake --build out/build/x64-release
```

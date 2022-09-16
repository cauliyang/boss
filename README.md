[![Ubuntu](https://github.com/cauliyang/boss/actions/workflows/linux.yml/badge.svg)](https://github.com/cauliyang/boss/actions/workflows/linux.yml)
[![Style](https://github.com/cauliyang/boss/actions/workflows/style.yml/badge.svg)](https://github.com/cauliyang/boss/actions/workflows/style.yml)
[![c++20](https://img.shields.io/badge/C++-c%2B%2B20-green)](https://en.cppreference.com/w/cpp/20)
[![CodeQL](https://github.com/cauliyang/boss/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/cauliyang/boss/actions/workflows/codeql-analysis.yml)
[![License](https://img.shields.io/github/license/ylab-hi/BINARY)](https://github.com/cauliyang/boss/blob/main/LICENSE)
![compiler](https://img.shields.io/badge/Compiler-GCC10%20%7C%20GCC11%20%7C%20GCC12-green)

# BioinfOrmaticS toolboxeS aka BOSS

## Current Tools

| Tool                    | Description                         | Status                                                         |
|-------------------------|-------------------------------------|----------------------------------------------------------------|
| [boss-fqsp](#boss-fqsp) | Fastq Splitter for Paried End Reads | ![boss-fqsp](https://img.shields.io/badge/Version-1.0.0-green) |

## Usage

### boss-fqsp

```console
boss-fqsp -h
```

## How to build

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/boos-fqsp --help
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/BossTests
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.

### Build the documentation

The documentation is automatically built and [published](https://thelartians.github.io/ModernCppStarter) whenever
a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is
created.
To manually build documentation, call the following command.

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target BossDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -S boss_all -B build
cmake --build build

# run tests
./build/test/BossTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/boss-fqsp --help
# build docs
cmake --build build --target BossDocs
```

### Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional
tools on-demand through CMake configuration arguments.
The following are currently supported.

#### Sanitizers

Sanitizers can be enabled by configuring CMake
with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of
those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS`
variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.

## Acknowledgements

- https://github.com/samtools/htslib/blob/develop/htslib
- https://github.com/doctest/doctest
- https://github.com/jarro2783/cxxopts
- https://github.com/gabime/spdlog
- https://github.com/TheLartians/ModernCppStarter
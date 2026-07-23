# Project description
This repository is designed to manage and compile course experiments and exercises for **Data Structures** using CMake. It supports a hybrid development environment of both **C** and **C++** (C11 and C++20 standards). 

## Key Features
* **Modular Library Architecture**: Common data structure implementations (located in `src/common/`) are automatically built into a static library (`ds_lib`), separating reusable core logic from individual chapter experiments.
* **Auto-Scanning Build System**: You do not need to manually edit `CMakeLists.txt` when adding new exercise files under `src/ch*/`. Any `.c` or `.cpp` file in the chapter directories is automatically detected, compiled, and linked against `ds_lib`.
* **Conflict-Free Target Naming**: Executable target names are automatically prefixed with their directory names (e.g., `src/ch2/link_list_test.cpp` becomes `ch2_link_list_test`), preventing name collisions across different chapters.
* **CMake Presets Support**: Fully integrated with `CMakePresets.json` to simplify configuring and building across different toolchains (Ninja, MinGW, and MSVC).
* **Robust Code Quality**: Features strict compiler warning flags (for both GCC/Clang and MSVC) to help catch common pointer, memory management, and type-mismatch bugs early.
* **Editor/LSP Integration**: Automatically generates and syncs `compile_commands.json` to the project root for precise code completion and diagnostics when using language servers like `clangd` in Vim/Neovim or VS Code.

## Project Structure
```text
DataStructures/
├── CMakeLists.txt         # Core build configuration
├── CMakePresets.json      # Pre-configured build/configure environments
├── .gitignore             # Git ignore rules
├── LICENSE                # Project license
├── README.md              # Project documentation
├── include/               # Public header files (.h / .hpp)
│   ├── linear_list.h
│   └── link_list.h
├── ref/                   # Reference materials, lecture notes, or PDFs
└── src/                   # Source files organized by common implementations and chapters
    ├── common/            # Common data structure implementations (compiled into ds_lib)
    │   ├── linear_list.cpp
    │   └── link_list.cpp
    ├── ch1/               # Chapter 1 Exercises
    │   └── fibonacci_sequence.cpp
    └── ch2/               # Chapter 2 Exercises
        ├── linear_list_test.cpp
        └── link_list_test.cpp
```

> A friendly reminder about potential naming conflicts: 
> 
> When using this automatic compilation mechanism, ensure that filenames in different chapter directories are not duplicated.
> 
> For example: If you write code in both `src/ch1/main.cpp` and `src/ch2/main.cpp`, the generated targets will both be named `main`, which will cause a CMake conflict error. It is recommended to give each experimental file a unique name, such as `ch1_seq_list.cpp` and `ch2_link_list.cpp`.

# Build commands
Since this project utilizes CMake Presets, you can configure and build the project using the simplified preset names without manually specifying generators or build directories.

## Prerequisites
Make sure you have CMake (3.15+) and at least one of the following toolchains installed and added to your system PATH:

- Ninja + LLVM/Clang (Recommended for the fastest build speeds and excellent clangd compatibility)

- MinGW (GCC)

- MSVC (Visual Studio 2022)

## Building with Ninja (Recommended)
Debug Mode (with debugging symbols):

```Bash
# Configure the project (generates compile_commands.json in the build directory)
cmake --preset ninja-debug

# Build all targets
cmake --build --preset ninja-debug
```

Release Mode (optimized):

```Bash
cmake --preset ninja-release
cmake --build --preset ninja-release
```

## Building with MinGW Makefiles
Debug Mode:

```Bash
cmake --preset mingw-debug
cmake --build --preset mingw-debug
```

Release Mode:

```Bash
cmake --preset mingw-release
cmake --build --preset mingw-release
```

## Building with Visual Studio (MSVC)
Since MSVC is a multi-configuration generator, configuration is done once, and the build type is determined at the build step:

```Bash
# Configure the project
cmake --preset msvc

# Build Debug version
cmake --build --preset msvc-debug

# Build Release version
cmake --build --preset msvc-release
```

## Compiling & Running a Specific Exercise
To avoid rebuilding the entire project, you can compile a single target (the filename without the extension) by using the `--target` flag.

*Note: Target names are automatically formatted as <chapter>_<filename> (e.g., src/ch2/link_list_test.cpp -> ch2_link_list_test).*

```bash
# Example: Compile only 'ch2_link_list_test'
cmake --build --preset ninja-debug --target ch2_link_list_test

# Run the compiled executable (Ninja Debug build path example)
./build/ninja/debug/ch2_link_list_test.exe
```
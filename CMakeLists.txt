cmake_minimum_required(VERSION 3.16)
project(data-structure 
VERSION 1.0.0
LANGUAGES C CXX
DESCRIPTION "A data structures learning project, involving exercices and experiments."
)

# ============================================
# 2. Language standard and compilation options
# ============================================
# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Generate compile_commands.json for cland lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warnning options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # C++
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
        $<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
        $<$<COMPILE_LANGUAGE:CXX>:-Wshadow>
        $<$<COMPILE_LANGUAGE:CXX>:-Wconversion>
    )
    # C
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:-Wall>
        $<$<COMPILE_LANGUAGE:C>:-Wextra>
        $<$<COMPILE_LANGUAGE:C>:-Wpedantic>
        $<$<COMPILE_LANGUAGE:C>:-Wshadow>
        $<$<COMPILE_LANGUAGE:C>:-Wconversion>
        $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
    )
elseif(MSVC)
    add_compile_options(/W4 /WX-)
endif()

# Automatically copy compile_commands.json so that LSPs such as Clangd can function correctly.
# Do not copy compile_commands.json in MSVC to prevent CMake errors.
if (NOT CMAKE_GENERATOR MATCHES "Visual Studio")
	if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
		add_custom_target(
			CopyCompileCommands ALL
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${CMAKE_BINARY_DIR}/compile_commands.json
				${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
			DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
			COMMENT "Copy compile_commands.json to the project source directory."
		)
	endif()
endif()

# If no build type is specified (not a multi-configuration generator like MSVC), 
# then debug mode is used by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# =====================
# 3. Include head files
# =====================
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ==========================================
# 4. Automated scanning and building targets
# ==========================================
# Scan all .c and .cpp files in ./src/
file(GLOB_RECURSE ALL_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Circularly compile every source file, including the new
foreach(source_file IN LISTS ALL_SOURCES)
    # Get the filename (excluding path and extension) as the name of the executable
    # Get the file extension, to distinguish files in the log
    get_filename_component(exe_name "${source_file}" NAME_WE)
    get_filename_component(file_ext "${source_file}" EXT)
    # Add compilation targets
    add_executable("${exe_name}""${source_file}")
    # Print log
    file(RELATIVE_PATH relative_src "${CMAKE_CURRENT_SOURCE_DIR}" "${source_file}")
    message(STATUS "Found source: src/${exe_name}${file_ext} -> Target: ${exe_name}")
endforeach()
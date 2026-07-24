cmake_minimum_required(VERSION 3.16)
project(data-structure 
VERSION 1.0.0
LANGUAGES C CXX
DESCRIPTION "A data structures learning project, involving exercices and experiments."
)

# ============================================
# 1. Language standard and compilation options
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

# ==========================================
# 2. Automated scanning and building targets
# ==========================================
# 2-1. Compile the Common Data Structure Library (ds_lib)
# Scan all implementation files under src/common/ (or explicitly specify src/common/link_list.cpp)
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp")
# Create the static library target ds_lib
add_library(ds_lib STATIC ${COMMON_SOURCES})
# Add the static library target to ds_lib Associate header file directories:
target_include_directories(ds_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 2-2. Automated Scanning and Compilation of Experiment Targets (Exes) for Each Chapter
# Note: Exclude the src/common directory; only scan the chapter's experiment code!
file(GLOB_RECURSE EXPERIMENT_SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ch*/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ch*/*.cpp"
)

# Circularly compile every source file, including the new
foreach(source_file IN LISTS EXPERIMENT_SOURCES)
    # Get path relative to src directory, e.g., "array/vector.cpp" -> "array/vector"
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}/src" "${source_file}")
    get_filename_component(rel_dir "${rel_path}" DIRECTORY)
    get_filename_component(filename_we "${rel_path}" NAME_WE)

    # Sanitize name for CMake Target: convert "array/vector" into "array_vector"
    if(rel_dir)
        string(REPLACE "/" "_" target_prefix "${rel_dir}")
        set(target_name "${target_prefix}_${filename_we}")
    else()
        set(target_name "${filename_we}")
    endif()

    # Create target
    add_executable("${target_name}" "${source_file}")
    
    # Automatically link common data structure libraries (ds_lib will automatically pass the include directory, no need to manually write target_include_directories)
    target_link_libraries("${target_name}" PRIVATE ds_lib)

    message(STATUS "Configured Target: ${target_name} <- src/${rel_path}")
endforeach()
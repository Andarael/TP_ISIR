# Declare CMAKE required version.
cmake_minimum_required(VERSION 3.13)

# Project name.
project(TP_ISIR)


# Create paths.
set(PATH_SRC "src")
set(PATH_LIB "lib")
set(CONFIGS "Debug;Release")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/x64)

message("Copying all sources...")
file(GLOB_RECURSE SOURCES ${PATH_SRC}/*.cpp ${PATH_SRC}/*.cu ${PATH_SRC}/*.hpp ${PATH_SRC}/*.h)
foreach(SRC IN ITEMS ${SOURCES})
    get_filename_component(SRC_PATH "${SRC}" PATH)
    file(RELATIVE_PATH SRC_PATH_REL "${CMAKE_SOURCE_DIR}/${PATH_SRC}" "${SRC_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "/${SRC_PATH_REL}")
	# Create group to make visual studio filters.
    source_group("Sources${GROUP_PATH}" FILES "${SRC_PATH_REL}")
endforeach()

# Compiler options #1.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CONFIGURATION_TYPES ${CONFIGS} CACHE STRING "" FORCE)
set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set link directories.
link_directories(${PATH_LIB}/assimp)

# Declare executable with source files.
add_executable(TP_ISIR ${SOURCES})

# Includes directories.
target_include_directories(TP_ISIR PRIVATE ${PATH_SRC})
target_include_directories(TP_ISIR PRIVATE ${PATH_LIB})
target_include_directories(TP_ISIR PRIVATE ${PATH_LIB}/assimp)
target_include_directories(TP_ISIR PRIVATE ${PATH_LIB}/glm)
target_include_directories(TP_ISIR PRIVATE ${PATH_LIB}/stb)

# Link libs.
target_link_libraries(TP_ISIR PRIVATE assimp-vc142-mt)

# Copy DLL to output.
# Debug
add_custom_command(TARGET TP_ISIR POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${PATH_LIB}/zlib1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/zlib1.dll)
add_custom_command(TARGET TP_ISIR POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${PATH_LIB}/assimp/assimp-vc142-mt.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/assimp-vc142-mt.dll)
# Release
add_custom_command(TARGET TP_ISIR POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${PATH_LIB}/zlib1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/zlib1.dll)
add_custom_command(TARGET TP_ISIR POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/${PATH_LIB}/assimp/assimp-vc142-mt.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/assimp-vc142-mt.dll)

# Compiler options #2.
if(MSVC)
	#General.
    target_compile_options(TP_ISIR PRIVATE "/W3")  # Warning level 3.
	target_compile_options(TP_ISIR PRIVATE "/WX-")  # Warnings as errors.
	target_compile_options(TP_ISIR PRIVATE "/MP")  # Multicore compilation.
	target_compile_options(TP_ISIR PRIVATE "/sdl") # SDL check.
	target_compile_options(TP_ISIR PRIVATE "/openmp") # OpenMP check.
	target_compile_options(TP_ISIR PRIVATE "/execution-charset:utf-8") # To display Angstrom symbol for example.
	#target_compile_options(TP_ISIR PRIVATE "/DEBUG:FASTLINK") # https://devblogs.microsoft.com/cppblog/debugfastlink-for-vs2015-update-1/

	#Optimization.
	target_compile_options(TP_ISIR PRIVATE "$<$<CONFIG:Release>:/O2>")
	target_compile_options(TP_ISIR PRIVATE "$<$<CONFIG:Release>:/Ob2>")
	target_compile_options(TP_ISIR PRIVATE "$<$<CONFIG:Release>:/Ot>")
	target_compile_options(TP_ISIR PRIVATE "$<$<CONFIG:Release>:/Oi>")


	#Code Generation.
	target_compile_options(TP_ISIR PRIVATE "/fp:fast") # Floating Point Model.

	#Preprocessor definitions
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DNOMINMAX)

	# Set default startup project.
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TP_ISIR)
	#set_target_properties(TP_ISIR PROPERTIES VS_GLOBAL_DisableFastUpToDateCheck false)
endif()

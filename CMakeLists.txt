cmake_minimum_required(VERSION 3.14.0)
project(Virtual_City_Explorer)

set (CMAKE_CXX_STANDARD 17)

# Prevent In Source Builds
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-Source Builds are not recommended. Please Create a build/ directory and run CMake specifying Build Directory." )
endif()

# Specify Stack Size for MinGW
if (MINGW)
	set (CMAKE_CXX_FLAGS "-Wl,--stack,16388608")
endif(MINGW)

# Include Directories
include_directories(inc)

# Source Files
file(GLOB SOURCES "src/*.cpp" "*.cpp" "Resource.obj")

# Executable
add_executable(virtual_explorer ${SOURCES})

# Libraries :

if(WIN32)
	set(LIBRARY
	SOIL
	glew32
	opengl32
	glfw3dll
	gdi32
	winmm
	freetype
	)
endif(WIN32)

if(UNIX)
	set(LIBRARY
	SOIL
	glfw
	GL
	GLEW
	pthread
	Xi
	X11 
	dl
	freetype
	)
endif(UNIX)

target_link_libraries(
	virtual_explorer
	${LIBRARY}
)

# COPY Assets to Build Directory
file(COPY font model shader res DESTINATION ${CMAKE_BINARY_DIR})
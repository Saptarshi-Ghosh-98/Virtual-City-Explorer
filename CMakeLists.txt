cmake_minimum_required(VERSION 3.14.0)
project(Virtual_City_Explorer)

set (CMAKE_CXX_STANDARD 17)

# Specify Stack Size for MinGW
if (MINGW)
	set (CMAKE_CXX_FLAGS "-Wl,--stack,16388608")
endif(MINGW)

# Include Directories
include_directories(inc)

# Source Files
file(GLOB SOURCES "src/*.cpp" "*.cpp")

# Executable
add_executable(virtual_explorer ${SOURCES})

# Libraries :

if(WIN32)
	set(LIBRARY 
	glew32
	opengl32
	glfw3dll
	gdi32
	winmm)
endif(WIN32)

if(UNIX)
	set(LIBRARY
	glfw
	GL
	GLEW
	pthread
	Xi
	X11 
	dl)
endif(UNIX)

target_link_libraries(
	virtual_explorer
	${LIBRARY}
)

# COPY Assets to Build Directory
file(COPY Model shader DESTINATION ${CMAKE_BINARY_DIR})
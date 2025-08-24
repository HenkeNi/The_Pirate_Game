cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
	CACHE STRING "Vcpkg toolchain file")

project(The_Pirate_Game VERSION 0.0.1 LANGUAGES CXX)
	
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# vcpkg dependencies	
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)


# --- Engine Library ---
file(GLOB_RECURSE Engine_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/*.inl"
	)

add_library(Engine STATIC ${Engine_SOURCES})
target_include_directories(Engine PUBLIC src/engine)

# Automatically links to specified libs
target_link_libraries(Engine PRIVATE
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
	SDL3_image::SDL3_image
)

# --- Game ---
file(GLOB_RECURSE Game_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.inl"
)

add_executable(Game ${Game_SOURCES})

target_sources(Game PRIVATE ${Game_SOURCES})

target_include_directories(Game PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Game PRIVATE Engine)

# Preprocessor defines
target_compile_definitions(Game PRIVATE)

# Add directories with any random loose .dlls or .libs here
target_link_directories(Game PRIVATE)

# Set configuration properties
if(MSVC)
	target_compile_options(Game PRIVATE
		/W3
		/MP # multithreaded build
		/WX # warnings as errors
		/ZI # program database for edit and continue	
	)
else()
	target_compile_options(Game PRIVATE
		-Wall
		-Wextra
		-Wpedantic
	)
endif()

# Set as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)

# Filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/engine" PREFIX "Engine" FILES ${Engine_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/game" PREFIX "Game" FILES ${Game_SOURCES})


# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
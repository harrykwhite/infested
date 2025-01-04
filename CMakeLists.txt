cmake_minimum_required(VERSION 3.28)

project(infested)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(zf4)

find_package(glfw3 CONFIG REQUIRED)

add_executable(infested
	src/entry.cpp
	src/sprites.cpp
	src/component_types.cpp
	src/scenes/scene_types.cpp
	src/scenes/title_screen.cpp
	src/scenes/world.cpp
	src/ents/ents.cpp
	src/ents/player.cpp
	src/ents/enemy.cpp
	src/ents/gun.cpp
	src/ents/bullet.cpp

	src/asset_indexes.h
	src/sprites.h
	src/component_types.h
	src/scenes/scene_types.h
	src/scenes/title_screen.h
	src/scenes/world.h
)

target_include_directories(infested PRIVATE
    zf4/zf4/include
    zf4/zf4_common/include
	zf4/vendor/glad/include
)

target_link_libraries(infested PRIVATE zf4 zf4_common glfw)

target_compile_definitions(infested PRIVATE GLFW_INCLUDE_NONE)

target_compile_options(infested PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -pedantic>
)

add_dependencies(infested zf4_asset_packer)

add_custom_command(TARGET infested POST_BUILD
	COMMAND $<TARGET_FILE:zf4_asset_packer> ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}
)

cmake_minimum_required(VERSION 3.14)

project(body_isosurface)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(main ${SOURCES})

target_include_directories(main PUBLIC include include/imgui)
target_link_libraries(main PUBLIC ${CMAKE_SOURCE_DIR}/lib/libglfw3.a GL X11 pthread Xrandr Xi dl GLEW)

cmake_minimum_required(VERSION 3.14)

project(body_isosurface)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3")

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(pybind11 URL https://github.com/pybind/pybind11/archive/v2.6.2.tar.gz)

FetchContent_GetProperties(pybind11)

if(NOT pybind11_POPULATED)
    FetchContent_Populate(pybind11)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()

FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# main process
add_executable(main ${SOURCES})

target_include_directories(main PUBLIC include include/imgui)
target_link_libraries(main ${CMAKE_SOURCE_DIR}/lib/libglfw3.a GL X11 pthread Xrandr Xi dl GLEW)

# pybind11
pybind11_add_module(_volume ${CMAKE_SOURCE_DIR}/volume_wrapper.cpp ${CMAKE_SOURCE_DIR}/src/Volume.cpp)

target_include_directories(_volume PUBLIC include extern/pybind11/include)

pybind11_add_module(_iso_surface ${CMAKE_SOURCE_DIR}/iso_surface_wrapper.cpp ${CMAKE_SOURCE_DIR}/src/IsoSurface.cpp ${CMAKE_SOURCE_DIR}/src/Volume.cpp)

target_include_directories(_iso_surface PUBLIC include extern/pybind11/include)

# google test
enable_testing()

add_executable(unit_test ${CMAKE_SOURCE_DIR}/unit_test.cpp ${CMAKE_SOURCE_DIR}/src/Volume.cpp ${CMAKE_SOURCE_DIR}/src/IsoSurface.cpp)

include(GoogleTest)
target_include_directories(unit_test PUBLIC include include/imgui)
target_link_libraries(unit_test gtest_main)

gtest_discover_tests(unit_test)

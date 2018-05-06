cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(moofoopeg VERSION 0.1.0 LANGUAGES CXX)

add_executable(moofoopeg main.cpp)

add_subdirectory(SFML-2.3)

# find_package(SFML 2.3 COMPONENTS graphics window system audio REQUIRED)

# target_include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(moofoopeg PRIVATE sfml-graphics sfml-window sfml-system)

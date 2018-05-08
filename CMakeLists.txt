cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(moofoopeg VERSION 0.1.0 LANGUAGES CXX)

add_executable(moofoopeg main.cpp)

set_target_properties(moofoopeg PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

add_subdirectory(SFML-2.4)

target_link_libraries(moofoopeg PRIVATE sfml-graphics sfml-window sfml-system)

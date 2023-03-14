cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(moofoopeg VERSION 0.1.0 LANGUAGES CXX)

set(EXECUTABLE_NAME moofoopeg)
set(SOURCES "src/main.cpp" "src/field.cpp" "src/game.cpp" "src/renderer.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-system sfml-graphics sfml-window)

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/assets)

cmake_minimum_required(VERSION 2.8)
project(editor)

# header file
include_directories(./ ./src)

# target
add_library(Editor src/editor.cc)
add_executable(Main main.cc)
target_link_libraries(Main Editor)

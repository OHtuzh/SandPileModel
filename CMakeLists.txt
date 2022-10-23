cmake_minimum_required(VERSION 3.23)
project(labwork_3_OHtuzh)

set(CMAKE_CXX_STANDARD 14)

add_library(
        program-arguments
        my_libraries/ProgramArguments/program_arguments.h
        my_libraries/ProgramArguments/program_arguments.cpp
)

add_library(
        bmp-file-writer
        my_libraries/BmpFile/bmp_file.h
        my_libraries/BmpFile/bmp_file.cpp
)

add_executable(labwork_3_OHtuzh main.cpp)

target_link_libraries(labwork_3_OHtuzh PRIVATE program-arguments bmp-file-writer)

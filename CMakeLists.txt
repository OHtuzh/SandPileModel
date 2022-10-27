cmake_minimum_required(VERSION 3.23)
project(labwork_3_OHtuzh)

set(CMAKE_CXX_STANDARD 17)

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

add_library(
        sand-pile-model
        my_libraries/SandPileModel/sand_pile_model.h
        my_libraries/SandPileModel/sand_pile_model.cpp
)

add_executable(labwork_3_OHtuzh main.cpp)

target_link_libraries(labwork_3_OHtuzh PRIVATE program-arguments sand-pile-model bmp-file-writer)

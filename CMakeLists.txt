cmake_minimum_required(VERSION 3.16)
project(minish LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SRC
    src/*.c
)

add_executable(minish ${SRC})

target_compile_options(minish PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

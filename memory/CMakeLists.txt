add_library(
        memory
        mem_core.cc
)

target_include_directories(
        memory
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes
)
target_compile_options(memory PRIVATE -Wall -Werror -std=c++20)
add_library(
        record
        record.cc
)

target_include_directories(
        record
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/record
)
target_compile_options(
        record
        PRIVATE
        #-Wall -Werror
        -std=c++20)
target_link_libraries(
        record
        PUBLIC
        type
)
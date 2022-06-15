add_library(
        schema
        record.cc
        schema.cc
        ../bp_tree/data.cc
        field.cpp
)

target_include_directories(
        schema
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/schema
        ${PROJECT_SOURCE_DIR}/includes/bp_tree
)
target_compile_options(
        schema
        PRIVATE
        #-Wall -Werror
        -std=c++20)
target_link_libraries(
        schema
        PUBLIC
        type
)
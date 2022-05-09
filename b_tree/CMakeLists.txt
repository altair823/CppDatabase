add_library(
        b_tree
        node.cc
)

target_include_directories(
        b_tree
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/storage/b_tree
)
target_compile_options(
        b_tree
        PRIVATE
        #-Wall -Werror
        -std=c++20
)
target_link_libraries(
        b_tree
        PUBLIC
        type
)
target_link_libraries(
        b_tree
        PUBLIC
        schema
)
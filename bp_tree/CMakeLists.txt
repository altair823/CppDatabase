add_library(
        bp_tree
        db_io.cc
)

target_include_directories(
        bp_tree
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/storage/bp_tree
)

target_compile_options(
        bp_tree
        PRIVATE
        #-Wall -Werror
        -std=c++20)

target_link_libraries(
        bp_tree
        PUBLIC
        type
        memory
)
add_library(
        stream
        db_fstream.cc
)

target_include_directories(
        stream
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/stream
)
target_compile_options(
        stream
        PRIVATE
        #-Wall -Werror
        -std=c++20)
target_link_libraries(
        stream
        PUBLIC
        memory
)
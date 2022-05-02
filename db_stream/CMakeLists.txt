add_library(
    db_stream
    db_stream.cc
)

target_include_directories(db_stream PUBLIC ${PROJECT_SOURCE_DIR}/includes)
target_compile_options(db_stream PRIVATE -Wall -Werror -std=c++20)
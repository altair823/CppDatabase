add_library(
        type
        type.cc
        f_datetime.cc
        f_string.cc
        ../includes/type/f_int.h f_int.cpp)

target_include_directories(
        type
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/type
)
target_compile_options(
        type
        PRIVATE
        #-Wall -Werror
        -std=c++20)
target_link_libraries(
        type
        PUBLIC
        memory
)
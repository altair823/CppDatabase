add_library(
        type
        field_data.cc
        f_datetime.cc
        f_string.cc
        f_date.cc
)

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
cmake_minimum_required(VERSION 3.22)

project(
        CppDatabase
        VERSION 0.1
)


add_executable(
        CppDatabase
        main.cpp
)
target_compile_options(CppDatabase
        PUBLIC
        #-Wall -Werror
        -D_FILE_OFFSET_BITS=64
        -D_LARGEFILE64_SOURCE
        -std=c++20)

add_subdirectory(tests)

add_subdirectory(type)
add_subdirectory(memory)
add_subdirectory(schema)
add_subdirectory(bp_tree)
target_link_libraries(CppDatabase PUBLIC type)
target_link_libraries(CppDatabase PUBLIC memory)
target_link_libraries(CppDatabase PUBLIC schema)
target_link_libraries(CppDatabase PUBLIC bp_tree)
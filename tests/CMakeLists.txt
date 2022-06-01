cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        test
        test_util.cc
        f_datetime_test.cc
        f_string_test.cc
        record_test.cc schema_test.cc table_test.cpp bp_tree_test.cpp db_io_test.cc mem_core_test.cpp index_node_test.cpp)
target_link_libraries(
        test
        gtest_main
        type
        schema
        bp_tree
)
target_include_directories(
        test
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/test
        ${PROJECT_SOURCE_DIR}/includes
)


include(GoogleTest)
gtest_discover_tests(test)

target_compile_options(
        test
        PRIVATE
        #-Wall -Werror
        -D_FILE_OFFSET_BITS=64
        -D_LARGEFILE64_SOURCE
        -std=c++20
)
#set_target_properties(test PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
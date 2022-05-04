cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

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
        record_test.cc)
target_link_libraries(
        test
        gtest_main
        type
        record
)
target_include_directories(
        test
        PUBLIC
        ${PROJECT_SOURCE_DIR}/includes/test
)

include(GoogleTest)
gtest_discover_tests(test)

target_compile_options(
        test
        PRIVATE
        #-Wall -Werror
        -std=c++20)

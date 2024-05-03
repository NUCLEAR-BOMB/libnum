cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(libnum VERSION 0.1 LANGUAGES CXX)

option(LIBNUM_BUILD_TEST "Build the test target" ${PROJECT_IS_TOP_LEVEL})
option(LIBNUM_ENABLE_SANITIZERS "Enable sanitizers for test project" ${PROJECT_IS_TOP_LEVEL})
option(LIBNUM_ENABLE_CLANG_TIDY "Enable the clang-tidy analyzer" ${PROJECT_IS_TOP_LEVEL})

add_library(libnum INTERFACE
    "include/libnum/uint128.hpp"
    "include/libnum/int128.hpp"

    "include/libnum/detail/intrinsics.hpp"
    "include/libnum/detail/preprocessor.hpp"
    "include/libnum/detail/traits.hpp"
)

if (NOT PROJECT_IS_TOP_LEVEL)
    set(include_as_system "SYSTEM")
endif()

target_include_directories(libnum ${include_as_system} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(libnum INTERFACE cxx_std_17)

if (LIBNUM_BUILD_TEST)
    add_subdirectory(test)
endif()



include("${PROJECT_SOURCE_DIR}/cmake/warnings.cmake")

add_executable(libnum-test
	"main.cpp"
	
	"uint128.cpp"
)

target_add_warnings(libnum-test)
target_link_libraries(libnum-test PRIVATE libnum)

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
	SYSTEM
)
set(CMAKE_WARN_DEPRECATED FALSE CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(doctest)
unset(CMAKE_WARN_DEPRECATED CACHE)

target_link_libraries(libnum-test PRIVATE doctest::doctest)
target_compile_definitions(libnum-test PRIVATE DOCTEST_CONFIG_NO_EXCEPTIONS)

add_custom_target(run-libnum-test VERBATIM COMMAND "$<TARGET_FILE:libnum-test>")

if (LIBNUM_ENABLE_SANITIZERS)
    include("${PROJECT_SOURCE_DIR}/cmake/sanitizers.cmake")
    # target_add_sanitizer(libnum-test SANITIZERS Address Undefined)
endif()

if (LIBNUM_ENABLE_CLANG_TIDY)
    include("${PROJECT_SOURCE_DIR}/cmake/clang_tidy.cmake")
    set_target_properties(libnum-test PROPERTIES EXPORT_COMPILE_COMMANDS TRUE)
    add_clang_tidy_target(NAME clang-tidy
        CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
        EXTRA_ARG "-std=c++17" "-Wno-unknown-warning-option"
        USE_COLOR QUIET
    )
endif()


add_library(libnum-assembly-test
    "uint128.cpp"
)
target_link_libraries(libnum-assembly-test PRIVATE libnum)

find_package(Python3 QUIET COMPONENTS Interpreter)
if (NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "[assembly-test] Python3 Interpreter is not found")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(assembly_test_tag "msvc")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(assembly_test_tag "clang")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(assembly_test_tag "gcc")
endif()

add_custom_target(run-libnum-assembly-test VERBATIM
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/test/assembly/run-test.py" "$<TARGET_FILE:libnum-assembly-test>" "${assembly_test_tag}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test/assembly"
)

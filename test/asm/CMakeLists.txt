
add_library(libnum-asm-check-lib
    "uint128_check.cpp"
)
target_link_libraries(libnum-asm-check-lib PRIVATE libnum)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(libnum-asm-check-lib PRIVATE
        /wd4190 # 'identifier1' has C-linkage specified, but returns UDT 'identifier2' which is incompatible with C
    )
endif()

function (add_run_asm_check_target)
    find_package(Python3 QUIET COMPONENTS Interpreter)
    if (NOT Python3_Interpreter_FOUND)
        message(STATUS "[asm-check] Python3 Interpreter is not found")
        return()
    endif()

    add_custom_target(run-libnum-asm-check
        COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/test/asm/run-test.py" "$<TARGET_FILE:libnum-asm-check-lib>" "${PROJECT_SOURCE_DIR}/test/asm/uint128_check.cpp"
        VERBATIM
    )
endfunction()

add_run_asm_check_target()

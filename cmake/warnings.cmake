include_guard(GLOBAL)

function(target_add_warnings target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
        /Wall /WX /permissive-
		/wd4514 # 'function' : unreferenced inline function has been removed
        /wd4710 # 'function' : function not inlined
        /wd4866 # 'file(line_number)' compiler may not enforce left-to-right evaluation order for call to operator_name
        >)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
        -Wall # Enables all the warnings
        -Wextra # Enables some extra warning flags that are not enabled by -Wall
        -Wpedantic # Issue all the warnings demanded by strict ISO C and ISO C++
        -Wconversion # Warn for implicit conversions that may alter a value
        -Werror # Make all warnings into errors.
        -Wcast-align # Warn whenever a pointer is cast such that the required alignment of the target is increased
        -Wunused # -Wunused options combined
        -Wconversion # Warn for implicit conversions that may alter a value
        -Wdouble-promotion # Give a warning when a value of type float is implicitly promoted to double
        -Wold-style-cast # Warn for C style casting
        -Wshadow # Warn whenever a local variable or type declaration shadows another variable
        -Wno-sign-conversion # Don't warn for implicit conversions that may change the sign of an integer value
        -Wno-old-style-cast # Disable warning for using C-style casts
        -Wno-c++98-compat
        -Wno-pre-c++17-compat
        -Wno-pre-c++14-compat
        -Wno-unsafe-buffer-usage
        -Wno-ctad-maybe-unsupported
        -Wno-c++98-compat-bind-to-temporary-copy
        -Wno-global-constructors
        -Wno-c++98-compat-pedantic
        -Wno-zero-as-null-pointer-constant
        -Wno-shadow-field-in-constructor
        -Wno-self-assign-overloaded
        -Wno-assume
        >)
    endif()
endfunction()

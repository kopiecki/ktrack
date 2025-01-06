function(compiler_warnings_install TARGET_NAME)
    set(ARG_OPTIONS WARNINGS_AS_ERRORS)
    set(ARG_SINGLE "")
    set(ARG_MULTI "")
    cmake_parse_arguments(PARSE_ARGV 1 ARG "${ARG_OPTIONS}" "${ARG_SINGLE}"
                          "${ARG_MULTI}")

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wshadow
        -Wcast-align
        -Wunused
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2)

    if(ARG_WARNINGS_AS_ERRORS)
        set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
        set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
    endif()

    set(GCC_WARNINGS ${CLANG_WARNINGS} -Wmisleading-indentation
                     -Wduplicated-cond -Wduplicated-branches -Wlogical-op)

    if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS ${GCC_WARNINGS})
    else()
        message(
            AUTHOR_WARNING
                "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler."
        )
    endif()

    target_compile_options(${TARGET_NAME} PRIVATE ${PROJECT_WARNINGS})

    message(STATUS "Compiler warnings configured to: ${TARGET_NAME}")

endfunction()

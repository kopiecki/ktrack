set(CMAKE_SYSROOT        /)
set(CMAKE_C_COMPLILER    gcc)
set(CMAKE_CXX_COMPILER   g++)
set(CMAKE_LINKER         ld)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

macro(set_flags_for_build_type type flags)
    set(CMAKE_C_FLAGS_${type} ${flags})
    set(CMAKE_CXX_FLAGS_${type} ${flags})
endmacro()

set_flags_for_build_type(DEBUG   "-Og -g3 -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")
set_flags_for_build_type(RELEASE "-O3 -DNDEBUG")

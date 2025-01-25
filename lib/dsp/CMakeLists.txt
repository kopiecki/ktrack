set(target "dsp")
set(sources src/rng.c src/kf.c)

add_library(${target} STATIC ${sources})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIR})
target_link_libraries(${target} PRIVATE GSL::gsl GSL::gslcblas)

include(compiler/warnings-c)
compiler_warnings_install(${target})

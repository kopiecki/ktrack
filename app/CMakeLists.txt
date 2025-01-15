set(target ${PROJECT_NAME})
set(sources src/noise.c src/circ_buf.c src/engine.c src/main.c)

add_executable(${target} ${sources})

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIR})

target_link_libraries(${target} PRIVATE SDL2 SDL2_ttf GSL::gsl GSL::gslcblas)

include(compiler/warnings-c)
compiler_warnings_install(${target})

set(target "engine")
set(sources src/engine.c src/sig_handler.c src/text.c)

add_library(${target} STATIC ${sources})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIR})
target_link_libraries(${target} PRIVATE SDL2 SDL2_ttf)

include(compiler/warnings-c)
compiler_warnings_install(${target})

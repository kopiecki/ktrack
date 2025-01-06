set(target ${PROJECT_NAME})
set(sources src/engine.c src/main.c)

add_executable(${target} ${sources})

target_link_libraries(${target} PRIVATE SDL2 SDL2_ttf)

include(compiler/warnings-c)
compiler_warnings_install(${target})

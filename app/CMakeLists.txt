set(target ${PROJECT_NAME})
set(sources src/track.c src/app.c src/circ_buf.c src/main.c)

add_executable(${target} ${sources})

target_link_libraries(${target} PRIVATE dsp engine)

include(compiler/warnings-c)
compiler_warnings_install(${target})

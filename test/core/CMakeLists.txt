
# {SRC} variable comes from the upper level CMakeList
add_executable(test_Scene test_Scene.cpp
        ${SRC})

target_link_libraries(test_Scene
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib
        Catch2::Catch2)


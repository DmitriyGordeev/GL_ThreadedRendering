add_executable(RenderingThread
        gl_rendering_thread_test.cpp
        ${CORE_DIR}/Shaders.cpp
        ${UTILS_DIR}/Logger.cpp
        ${CORE_DIR}/Camera.cpp
        ${UTILS_DIR}/picoPNG.cpp
        ${UTILS_DIR}/IO.cpp
        ${CORE_DIR}/TextureLoader.cpp
        ${CORE_DIR}/InputSystem.cpp
        RObject.cpp)

target_link_libraries(RenderingThread
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib)
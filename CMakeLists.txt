cmake_minimum_required(VERSION 3.26)
project(ColliderGame)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_CXX_STANDARD 20)

# 1. project sources and modules
file(GLOB SRC
        core/*.cpp
        utils/*.cpp)
include_directories(core/)
include_directories(utils/)

# 2. third party includes
include_directories(thirdparty/glew-1.9.0-win64/glew-1.9.0/include)
include_directories(thirdparty/glm/glm/)
include_directories(thirdparty/SDL2-2.0.12/include)

# 2.5 Catch2 ---------------------------------------------------------
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
include_directories(${catch2_SOURCE_DIR}/include/)


# ------------------------------------------------------------------

# 3. OpenGL:
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

# 4. SDL2 library path
link_directories(thirdparty/SDL2-2.0.12/lib/x64/)

# 5. GLEW library path
link_directories(thirdparty/glew-1.9.0-win64/glew-1.9.0/lib)

add_subdirectory(test/)

# 6. Create executable
add_executable(ColliderGame main.cpp ${SRC})
target_link_libraries(ColliderGame
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib)

# ========== gl_test ===============
add_executable(TestGL gl_test.cpp
        core/Shaders.cpp
        utils/Logger.cpp
        core/Camera.cpp
        utils/picoPNG.cpp
        utils/IO.cpp
        core/TextureLoader.cpp)
target_link_libraries(TestGL
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib)


# ========== gl_rendering_thread_test ===============
add_executable(RenderingThread gl_rendering_thread_test.cpp
        core/Shaders.cpp
        utils/Logger.cpp
        core/Camera.cpp
        utils/picoPNG.cpp
        utils/IO.cpp
        core/TextureLoader.cpp
        core/Object.cpp)
target_link_libraries(RenderingThread
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib)



cmake_minimum_required(VERSION 3.26)
project(ColliderGame)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_CXX_STANDARD 20)

include_directories(thirdparty/glew-1.9.0-win64/glew-1.9.0/include)
include_directories(thirdparty/glm/glm/)
include_directories(thirdparty/SDL2-2.0.12/include)

# OpenGL:
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

# ----------------------------------------------------------------------
# SDL2 library:
link_directories(thirdparty/SDL2-2.0.12/lib/x64/)

# GLEW library:
link_directories(thirdparty/glew-1.9.0-win64/glew-1.9.0/lib)

add_executable(ColliderGame main.cpp)
target_link_libraries(ColliderGame
        ${OPENGL_LIBRARIES}
        SDL2.lib
        SDL2main.lib
        glew32.lib
        glew32s.lib)

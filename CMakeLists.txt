cmake_minimum_required(VERSION 3.13)
project(Percolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

include_directories(/usr/local/include glad KHR)
include_directories(${GLFW_INCLUDE_DIRS})


set(SOURCE_FILES src/cpp/main.cpp)
add_executable(Percolation src/cpp/main.cpp src/cpp/glad.c src/cpp/Shader.cpp include/Shader.h)
target_link_libraries (Percolation
        ${OPENGL_LIBRARIES}
        ${GLFW3_LIBRARY} glfw)
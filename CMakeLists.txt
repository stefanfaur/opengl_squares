cmake_minimum_required(VERSION 3.24)
project(opengl_squares C)

set(CMAKE_C_STANDARD 11)

add_executable(opengl_squares src/main.c)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


find_package(glfw3 3.3 REQUIRED)
target_link_libraries(opengl_squares glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(opengl_squares OpenGL::GL)

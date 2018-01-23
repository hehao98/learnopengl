CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

# configure a header file to pass some of the CMake settings
# to the source code.
configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_SOURCE_DIR}/config.h"
)

# Add necessary include directories
list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# Add necessary library directories
find_package(OpenGL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.2.1)

add_executable(glfwtest glfwtest.cpp glad.c)
target_link_libraries(glfwtest glfw ${OPENGL_gl_LIBRARY})

add_executable(triangle triangle.cpp glad.c)
target_link_libraries(triangle glfw ${OPENGL_gl_LIBRARY})
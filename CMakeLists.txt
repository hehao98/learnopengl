CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Add necessary include directories
list(APPEND INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# Add necessary library directories
find_package(OpenGL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.2.1)

add_executable(glfwtest glfwtest.cpp)
target_link_libraries(
        glfwtest
        glfw
        ${OPENGL_gl_LIBRARY}
)
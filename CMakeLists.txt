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

add_executable(triangle triangle.cpp glad.c Shader.cpp)
target_link_libraries(triangle glfw ${OPENGL_gl_LIBRARY})

add_executable(rectangle rectangle.cpp glad.c)
target_link_libraries(rectangle glfw ${OPENGL_gl_LIBRARY})

add_executable(2triangles1VBO 2triangles1VBO.cpp glad.c)
target_link_libraries(2triangles1VBO glfw ${OPENGL_gl_LIBRARY})

add_executable(2triangles2VBOs 2triangles2VBOs.cpp glad.c)
target_link_libraries(2triangles2VBOs glfw ${OPENGL_gl_LIBRARY})

add_executable(2difftriangles 2difftriangles.cpp glad.c)
target_link_libraries(2difftriangles glfw ${OPENGL_gl_LIBRARY})

add_executable(useshader useshader.cpp glad.c)
target_link_libraries(useshader glfw ${OPENGL_gl_LIBRARY})

add_executable(vertexcolor vertexcolor.cpp glad.c)
target_link_libraries(vertexcolor glfw ${OPENGL_gl_LIBRARY})

add_executable(UpsideDownTriangle UpsideDownTriangle.cpp Shader.cpp glad.c)
target_link_libraries(UpsideDownTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(HorizonalMovingTriangle HorizonalMovingTriangle.cpp Shader.cpp glad.c)
target_link_libraries(HorizonalMovingTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(VertexDataColoredTriangle VertexDataColoredTriangle.cpp Shader.cpp glad.c)
target_link_libraries(VertexDataColoredTriangle glfw ${OPENGL_gl_LIBRARY})

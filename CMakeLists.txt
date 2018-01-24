CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.2.1)

add_executable(glfwtest src/GettingStarted/glfwtest.cpp src/glad.c)
target_link_libraries(glfwtest glfw ${OPENGL_gl_LIBRARY})

add_executable(triangle src/GettingStarted/triangle.cpp src/glad.c src/Shader.cpp)
target_link_libraries(triangle glfw ${OPENGL_gl_LIBRARY})

add_executable(rectangle src/GettingStarted/rectangle.cpp src/glad.c)
target_link_libraries(rectangle glfw ${OPENGL_gl_LIBRARY})

add_executable(2triangles1VBO src/GettingStarted/2triangles1VBO.cpp src/glad.c)
target_link_libraries(2triangles1VBO glfw ${OPENGL_gl_LIBRARY})

add_executable(2triangles2VBOs src/GettingStarted/2triangles2VBOs.cpp src/glad.c)
target_link_libraries(2triangles2VBOs glfw ${OPENGL_gl_LIBRARY})

add_executable(2difftriangles src/GettingStarted/2difftriangles.cpp src/glad.c)
target_link_libraries(2difftriangles glfw ${OPENGL_gl_LIBRARY})

add_executable(useshader src/GettingStarted/useshader.cpp src/glad.c)
target_link_libraries(useshader glfw ${OPENGL_gl_LIBRARY})

add_executable(vertexcolor src/GettingStarted/vertexcolor.cpp src/glad.c)
target_link_libraries(vertexcolor glfw ${OPENGL_gl_LIBRARY})

add_executable(UpsideDownTriangle src/GettingStarted/UpsideDownTriangle.cpp src/Shader.cpp src/glad.c)
target_link_libraries(UpsideDownTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(HorizonalMovingTriangle src/GettingStarted/HorizonalMovingTriangle.cpp src/Shader.cpp src/glad.c)
target_link_libraries(HorizonalMovingTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(VertexDataColoredTriangle src/GettingStarted/VertexDataColoredTriangle.cpp src/Shader.cpp src/glad.c)
target_link_libraries(VertexDataColoredTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(TexturedTriangle src/GettingStarted/TexturedTriangle.cpp src/Shader.cpp src/glad.c)
target_link_libraries(TexturedTriangle glfw ${OPENGL_gl_LIBRARY})


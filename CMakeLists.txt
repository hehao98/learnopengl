CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/glfw-3.2.1)

################ Getting Started #################
add_executable(glfwtest src/GettingStarted/glfwtest.cpp src/glad.c)
target_link_libraries(glfwtest glfw ${OPENGL_gl_LIBRARY})

add_executable(triangle src/GettingStarted/triangle.cpp src/glad.c)
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

add_executable(
        UpsideDownTriangle
        src/GettingStarted/UpsideDownTriangle.cpp
        src/glad.c
)
target_link_libraries(UpsideDownTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(
        HorizonalMovingTriangle
        src/GettingStarted/HorizonalMovingTriangle.cpp
        src/glad.c
)
target_link_libraries(HorizonalMovingTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(VertexDataColoredTriangle
        src/GettingStarted/VertexDataColoredTriangle.cpp
        src/glad.c
        )
target_link_libraries(VertexDataColoredTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(
        TexturedTriangle
        src/GettingStarted/TexturedTriangle.cpp
        src/glad.c
)
target_link_libraries(TexturedTriangle glfw ${OPENGL_gl_LIBRARY})

add_executable(glmtest src/GettingStarted/glmtest.cpp)

add_executable(RotatedFace src/GettingStarted/RotatedFace.cpp src/glad.c)
target_link_libraries(RotatedFace glfw ${OPENGL_gl_LIBRARY})

add_executable(Going3D src/GettingStarted/Going3D.cpp src/glad.c)
target_link_libraries(Going3D glfw ${OPENGL_gl_LIBRARY})

add_executable(Box src/GettingStarted/Box.cpp src/glad.c)
target_link_libraries(Box glfw ${OPENGL_gl_LIBRARY})

add_executable(Camera src/GettingStarted/Camera.cpp src/glad.c)
target_link_libraries(Camera glfw ${OPENGL_gl_LIBRARY})
##################################################


#################### Lighting ####################
add_executable(BasicLightingScene src/Lighting/BasicLightingScene.cpp src/glad.c)
target_link_libraries(BasicLightingScene glfw ${OPENGL_gl_LIBRARY})

add_executable(Phong src/Lighting/Phong.cpp src/glad.c)
target_link_libraries(Phong glfw ${OPENGL_gl_LIBRARY})

add_executable(PhongInViewSpace src/Lighting/PhongInViewSpace.cpp src/glad.c)
target_link_libraries(PhongInViewSpace glfw ${OPENGL_gl_LIBRARY})

add_executable(Gouraud src/Lighting/Gouraud.cpp src/glad.c)
target_link_libraries(Gouraud glfw ${OPENGL_gl_LIBRARY})

add_executable(Material src/Lighting/Material.cpp src/glad.c)
target_link_libraries(Material glfw ${OPENGL_gl_LIBRARY})

add_executable(LightingMaps src/Lighting/LightingMaps.cpp src/glad.c)
target_link_libraries(LightingMaps glfw ${OPENGL_gl_LIBRARY})

add_executable(DirectionalLight src/Lighting/DirectionalLight.cpp src/glad.c)
target_link_libraries(DirectionalLight glfw ${OPENGL_gl_LIBRARY})

add_executable(Spotlight src/Lighting/Spotlight.cpp src/glad.c)
target_link_libraries(Spotlight glfw ${OPENGL_gl_LIBRARY})

add_executable(MultipleLights src/Lighting/MultipleLights.cpp src/glad.c)
target_link_libraries(MultipleLights glfw ${OPENGL_gl_LIBRARY})
##################################################


##################### Models #####################
add_executable(nanosuit src/ModelLoading/nanosuit.cpp src/glad.c)
target_link_libraries(nanosuit glfw ${OPENGL_gl_LIBRARY} assimp)
##################################################


################ Advanced OpenGL #################
add_executable(ObjectOutlining src/AdvancedOpenGL/ObjectOutlining.cpp src/glad.c)
target_link_libraries(ObjectOutlining glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(Blending src/AdvancedOpenGL/Blending.cpp src/glad.c)
target_link_libraries(Blending glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(PostProcessing-Inversion src/AdvancedOpenGL/PostProcessing-Inversion.cpp src/glad.c)
target_link_libraries(PostProcessing-Inversion glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(PostProcessing-GrayScale src/AdvancedOpenGL/PostProcessing-GrayScale.cpp src/glad.c)
target_link_libraries(PostProcessing-GrayScale glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(PostProcessing-KernelEffect src/AdvancedOpenGL/PostProcessing-KernelEffect.cpp src/glad.c)
target_link_libraries(PostProcessing-KernelEffect glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(PostProcessing-Blur src/AdvancedOpenGL/PostProcessing-Blur.cpp src/glad.c)
target_link_libraries(PostProcessing-Blur glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(PostProcessing-EdgeDetection src/AdvancedOpenGL/PostProcessing-EdgeDetection.cpp src/glad.c)
target_link_libraries(PostProcessing-EdgeDetection glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(SkyBox src/AdvancedOpenGL/SkyBox.cpp src/glad.c)
target_link_libraries(SkyBox glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(StaticReflection src/AdvancedOpenGL/StaticReflection.cpp src/glad.c)
target_link_libraries(StaticReflection glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(StaticRefraction src/AdvancedOpenGL/StaticRefraction.cpp src/glad.c)
target_link_libraries(StaticRefraction glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(DynamicReflection src/AdvancedOpenGL/DynamicReflection.cpp src/glad.c)
target_link_libraries(DynamicReflection glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(DynamicRefraction src/AdvancedOpenGL/DynamicRefraction.cpp src/glad.c)
target_link_libraries(DynamicRefraction glfw ${OPENGL_gl_LIBRARY} assimp)

add_Executable(DynamicEnvironmentMap src/AdvancedOpenGL/DynamicEnvironmentMap.cpp src/glad.c)
target_link_libraries(DynamicEnvironmentMap glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(GeometryShader src/AdvancedOpenGL/GeometryShader.cpp src/glad.c)
target_link_libraries(GeometryShader glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(Explosion src/AdvancedOpenGL/Explosion.cpp src/glad.c)
target_link_libraries(Explosion glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(NormalVisualing src/AdvancedOpenGL/NormalVisualizing.cpp src/glad.c)
target_link_libraries(NormalVisualing glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(Instancing src/AdvancedOpenGL/Instancing.cpp src/glad.c)
target_link_libraries(Instancing glfw ${OPENGL_gl_LIBRARY})

add_executable(AsteroidField src/AdvancedOpenGL/AsteroidField.cpp src/glad.c)
target_link_libraries(AsteroidField glfw ${OPENGL_gl_LIBRARY} assimp)

add_executable(GammaCorrection src/AdvancedOpenGL/GammaCorrection.cpp src/glad.c)
target_link_libraries(GammaCorrection glfw ${OPENGL_gl_LIBRARY})
##################################################

############### Advanced Lighting ################
add_executable(ShadowMapping src/AdvancedLighting/ShadowMapping.cpp src/glad.c)
target_link_libraries(ShadowMapping glfw ${OPENGL_gl_LIBRARY})

add_executable(NormalMapping src/AdvancedLighting/NormalMapping.cpp src/glad.c)
target_link_libraries(NormalMapping glfw ${OPENGL_gl_LIBRARY})
##################################################
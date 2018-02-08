//
// Created by hehao on 18-2-8.
//

#include <iostream>
#include <algorithm>

// GLM Math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLAD: A library that wraps OpenGL functions to make things easier
//       Note that GLAD MUST be included before GLFW
#include "glad/glad.h"
// GLFW: A library that helps us manage windows
#include <GLFW/glfw3.h>

// Wrapper classes to make things a little easier
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

int gScreenWidth = 800;
int gScreenHeight = 600;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

Camera gCamera;

// Perform necessary initialization.
// Returns pointer to a initialized window with OpenGL context set up
GLFWwindow *init();
// Do all the rendering stuff to the specific framebuffer
// returns -1 on error
int render(unsigned int framebuffer, Camera *camera, int width, int height);
// Sometimes user might resize the window. so the OpenGL viewport should be adjusted as well.
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// User input is handled in this function
void processInput(GLFWwindow *window);
// Mouse input is handled in this function
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double offsetX, double offsetY);
// Generate a cube map using the 6 file paths in the vector,
// Sequence: Right, left, top, bottom, back, front
unsigned int generateCubeMap(std::vector<std::string> facePaths);

std::vector<std::string> skyboxPaths = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg",
};
unsigned int skyboxTexture;
Texture *groundTexture;
Texture *ambientMap;
Texture *specularMap;
Texture *grassTexture;
Texture *transparentWindowTexture;

// Load shaders
Shader *objectShader;
Shader *transparentWindowShader;
Shader *skyboxShader;
Shader *dynamicRefractionShader;

unsigned int cubeVAO, groundVAO, planeVAO, skyboxVAO;

int main()
{
    GLFWwindow *window = init();
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW and OpenGL!" << std::endl;
        return -1;
    }

    skyboxTexture            = generateCubeMap(skyboxPaths);
    groundTexture            = new Texture("textures/ground.jpg");
    ambientMap               = new Texture("textures/container2.png");
    specularMap              = new Texture("textures/container2_specular.png");
    grassTexture             = new Texture("textures/grass.png");
    transparentWindowTexture = new Texture("textures/blending_transparent_window.png");

    objectShader            = new Shader("shaders/MultipleLights.vert", "shaders/Discard.frag");
    transparentWindowShader = new Shader("shaders/MultipleLights.vert", "shaders/BasicFrag.frag");
    skyboxShader            = new Shader("shaders/SkyboxShader.vert", "shaders/SkyboxShader.frag");
    dynamicRefractionShader = new Shader("shaders/MultipleLights.vert", "shaders/DynamicReflectionShader.frag");

    objectShader->use();
    objectShader->setInt("material.diffuse", 0);
    objectShader->setInt("material.specular", 1);
    objectShader->setInt("material.emission", 2);

    transparentWindowShader->use();
    transparentWindowShader->setInt("material.diffuse", 0);
    transparentWindowShader->setInt("material.specular", 1);
    transparentWindowShader->setInt("material.emission", 2);

    // Initialize skybox
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    unsigned int skyBoxVBO;
    glGenBuffers(1, &skyBoxVBO);
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // A set of cubeVertices to describe a cube(with normal vectors)
    float cubeVertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // Vertex Buffer Objects(VBO) are used to pass cubeVertices to GPU for the vertex objectShader
    unsigned int cubeVBO;
    // 1 is assigned as the unique ID to this cubeVBO
    glGenBuffers(1, &cubeVBO);

    // Vertex Array Objects(VAO) are used to store vertex attribute pointers
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // Tell OpenGL how tp interpret vertex data
    // Pass data to layout(location=0), each data 3 values, type float, no normalization,
    // with the stride as 6*sizeof(float)
    // location 0: vertex location
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Vertices data to describe a ground
    float groundVertices[] = {
            // Positions    // Normals        // Texture coordinates
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
    };
    unsigned int groundVBO;
    glGenBuffers(1, &groundVBO);
    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture coorinate to describe a plane
    float planeVertices[] = {
            // Positions    // Normals        // Texture coordinates
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Initialize a customized frame buffer, each for one cube surface
    unsigned int framebuffers[6], cubeMapColorBuffer, RBOs[6];
    glGenTextures(1, &cubeMapColorBuffer);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapColorBuffer);
    for (unsigned int i = 0; i < 6; ++i) {
        glGenFramebuffers(1, &framebuffers[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);

        // Generate texture;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        // Attach it to currently bind framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               cubeMapColorBuffer, 0);

        // Create a render buffer object to store depth and stencil buffer
        glGenRenderbuffers(1, &RBOs[i]);
        glBindRenderbuffer(GL_RENDERBUFFER, RBOs[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Bind the render buffer object to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOs[i]);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error: Incomplete framebuffer!" << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    gCamera.Position = glm::vec3(0.0f, 1.5f, 3.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate how much time since last frame
        auto currentFrame = (float)glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // Handle user input
        processInput(window);

        Camera camPos[6] = {
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f),         //posX
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 180.0f),       //negX
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, 90.0f),  //posY
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, -90.0f), //negY
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 90.0f),        //posZ
                Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), -90.0f),       //negZ
        };
        for (unsigned int i = 0; i < 6; ++i)
        {
            camPos[i].Zoom = 90.0f;
            render(framebuffers[i], &camPos[i], 1024, 1024);
        }

        // Render to default framebuffer
        render(0, &gCamera, gScreenWidth, gScreenHeight);

        // Draw the reflective box
        glBindVertexArray(cubeVAO);
        glm:: mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = gCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom),
                                                (float)gScreenWidth / gScreenHeight, 0.1f, 100.0f);
        dynamicRefractionShader->use();
        dynamicRefractionShader->setMat4("model", model);
        dynamicRefractionShader->setMat4("view", view);
        dynamicRefractionShader->setMat4("projection", projection);
        dynamicRefractionShader->setVec3("viewPos", gCamera.Position);
        dynamicRefractionShader->setVec3("reflectionBoxCenter", glm::vec3(0.0f, 1.0f, 0.0f));
        dynamicRefractionShader->setInt("reflectionBox", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int render(unsigned int framebuffer, Camera *camera, int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up view and projection matrix
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                                            (float)width / height, 0.1f, 100.0f);

    // Draw skybox first
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    skyboxShader->use();
    skyboxShader->setMat4("view", skyboxView);
    skyboxShader->setMat4("projection", projection);
    skyboxShader->setInt("skybox", 0);
    glDepthMask(GL_FALSE);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    // Draw the cubes
    objectShader->use();
    objectShader->setMat4("view", view);
    objectShader->setMat4("projection", projection);
    objectShader->setVec3("viewPos", camera->Position);

    ambientMap->useTextureUnit(0);
    specularMap->useTextureUnit(1);

    // Draw cubes
    glm::vec3 cubePositions[5] = {
            glm::vec3(0.0f,  0.0f, 0.0f),
            glm::vec3(2.0f, 0.5f, 2.0f),
            glm::vec3(2.0f, 0.5f, -2.0f),
            glm::vec3(-2.0f, 0.5f, -2.0f),
            glm::vec3(-2.0f, 0.5, 2.0f),
    };
    glBindVertexArray(cubeVAO);
    for (int i = 0; i < 5; ++i) {
        // Compute model transformations for each cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);

        objectShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Start to draw planes
    // Draw grasses
    glm::vec3 grassPositions[4] = {
            glm::vec3(2.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.5f, 2.0f),
            glm::vec3(-2.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 0.5f, -2.0f),
    };
    glBindVertexArray(planeVAO);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            model = glm::mat4(1.0f);

            model = glm::translate(model, grassPositions[i]);
            model = glm::rotate(model, glm::radians(45.0f) * j, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            objectShader->setMat4("model", model);
            grassTexture->useTextureUnit(0);
            // We set current texture to GL_CLAMP_TO_EDGE to prevent artifacts around the edge
            // from interpolating near texture borders
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    // Draw the ground
    glBindVertexArray(groundVAO);
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f));
    objectShader->use();
    objectShader->setMat4("model", model);
    groundTexture->useTextureUnit(0);
    // Use 0 to set the active texture to default texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw the transparent window
    // It must be drawn in the last so that all other objects can be blended with them
    // Also, if multiple transparent windows are involved, we MUST sort them and draw them
    // from farther to nearest to avoid depth testing issues.
    glBindVertexArray(planeVAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.8f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transparentWindowShader->use();
    transparentWindowShader->setMat4("view", view);
    transparentWindowShader->setMat4("projection", projection);
    transparentWindowShader->setVec3("viewPos", camera->Position);
    transparentWindowShader->setMat4("model", model);
    transparentWindowTexture->useTextureUnit(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return 0;
}

GLFWwindow *init()
{
    // Initialization of GLFW context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "Simple Scene", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    // Tell OpenGL the size of rendering window
    glViewport(0, 0, gScreenWidth, gScreenHeight);

    // Set the windows resize callback function
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // Set up mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    return window;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    gScreenWidth = width;
    gScreenHeight = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    // Variables needed to handle mouse input
    static float lastMouseX = 400.0f;
    static float lastMouseY = 300.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastMouseX = (float)xpos;
        lastMouseY = (float)ypos;
        firstMouse = false;
    }

    // Calculate mouse movement since last frame
    float offsetX = (float)xpos - lastMouseX;
    float offsetY = (float)ypos - lastMouseY;
    lastMouseX = (float)xpos;
    lastMouseY = (float)ypos;

    gCamera.ProcessMouseMovement(offsetX, offsetY);
}

void scrollCallback(GLFWwindow *window, double offsetX, double offsetY)
{
    gCamera.ProcessMouseScroll((float)offsetY);
}

unsigned int generateCubeMap(std::vector<std::string> facePaths)
{
    unsigned int tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < facePaths.size(); ++i)
    {
        unsigned char *image = stbi_load(facePaths[i].c_str(), &width, &height, &nrChannels, 0);
        if (image)
        {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image);
            stbi_image_free(image);
        }
        else
        {
            std::cout << "Cube map texture failed to load at path: " << facePaths[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return tid;
}
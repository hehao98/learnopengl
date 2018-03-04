//
// Created by 何昊 on 20180304.
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

bool gFunky = false;
bool gEmission = false;

// Perform necessary initialization.
// Returns pointer to a initialized window with OpenGL context set up
GLFWwindow *init();
// Sometimes user might resize the window. so the OpenGL viewport should be adjusted as well.
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// User input is handled in this function
void processInput(GLFWwindow *window);
// Mouse input is handled in this function
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double offsetX, double offsetY);

int main()
{
    GLFWwindow *window = init();
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW and OpenGL!" << std::endl;
        return -1;
    }

    Texture groundTexture("textures/ground.jpg");
    Texture ambientMap("textures/container2.png");
    Texture specularMap("textures/container2_specular.png");
    Texture emissionMap("textures/container2_emission.jpg");

    // Load shaders
    // Load light source shader
    Shader lightSourceShader("shaders/BasicLightingScene.vert",
                             "shaders/LightSource.frag");

    // Load Object Shader
    Shader objectShader("shaders/MultipleLights.vert", "shaders/MultipleLights.frag");
    objectShader.use();
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    objectShader.setInt("material.emission", 2);

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
    unsigned int cubeVAO;
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

    // For simplicity's sake in the future,
    // we put the light source in a separate cubeVAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices data to describe a plane
    float planeVertices[] = {
            // Positions    // Normals        // Texture coordinates
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
            0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
    };
    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);
    unsigned int planeVAO;
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

    glEnable(GL_DEPTH_TEST);
    // Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);
    // Enable anti-aliasing
    glEnable(GL_MULTISAMPLE);

    gCamera.Position = glm::vec3(0.0f, 1.5f, 3.0f);
    glm::vec3 lightSource = glm::vec3(1.2f, 0.5f, 1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate how much time since last frame
        auto currentFrame = (float)glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // Handle user input
        processInput(window);

        // All the rendering starts from here
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up view and projection matrix
        glm::mat4 view = gCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom),
                                                (float)gScreenWidth / gScreenHeight, 0.1f, 100.0f);

        // Draw the light source
        // Update light source position first
        lightSource = glm::vec3(4.0f * sinf(currentFrame), 0.5f, 4.0f * cosf(currentFrame));

        lightSourceShader.use();

        // Set up light properties
        if (gFunky) {
            float lightR = sinf((float) glfwGetTime() * 2.0f);
            float lightG = cosf((float) glfwGetTime() * 0.8f);
            float lightB = sinf((float) glfwGetTime() * 1.5f);
            lightColor = glm::vec3(lightR, lightG, lightB);
        }

        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setVec3("lightColor", lightColor);

        glm::mat4 lightShaderModel = glm::mat4(1.0f);
        lightShaderModel = glm::translate(lightShaderModel, lightSource);
        lightShaderModel = glm::scale(lightShaderModel, glm::vec3(0.05f));
        lightSourceShader.setMat4("model", lightShaderModel);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::vec3 spotLightTarget = glm::vec3(1.5f*cosf((float)glfwGetTime()), 0.0f,
                                              1.5f*sinf((float)glfwGetTime()));
        lightShaderModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));
        lightShaderModel = glm::scale(lightShaderModel, glm::vec3(0.05f));
        lightShaderModel = glm::rotate(lightShaderModel, atan2f(spotLightTarget.x, spotLightTarget.z),
                                       glm::vec3(0.0f, 1.0f, 0.0f));
        lightSourceShader.setMat4("model", lightShaderModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the cubes
        objectShader.use();
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        objectShader.setVec3("viewPos", gCamera.Position);

        // Set up material properties
        objectShader.setFloat("material.shininess", 32.0f);

        objectShader.setVec3("light.position", lightSource);
        objectShader.setVec3("light.ambient", lightColor * glm::vec3(0.05f));
        objectShader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));
        objectShader.setVec3("light.specular", lightColor * glm::vec3(1.0f));
        objectShader.setFloat("light.constant", 1.0f);
        objectShader.setFloat("light.linear", 0.022f);
        objectShader.setFloat("light.quadratic", 0.0010f);

        objectShader.setVec3("dirLight.direction", glm::vec3(-1.0f, -1.0f, 0.0f));
        objectShader.setVec3("dirLight.ambient", lightColor * glm::vec3(0.05f));
        objectShader.setVec3("dirLight.diffuse", lightColor * glm::vec3(0.3f));
        objectShader.setVec3("dirLight.specular", lightColor * glm::vec3(1.0f));

        objectShader.setVec3("spotLight.position", glm::vec3(0.0f, 3.0f, 0.0f));
        objectShader.setVec3("spotLight.direction", spotLightTarget - glm::vec3(0.0f, 3.0f, 0.0f));
        objectShader.setVec3("spotLight.ambient", lightColor * glm::vec3(0.1f));
        objectShader.setVec3("spotLight.diffuse", lightColor * glm::vec3(0.5f));
        objectShader.setVec3("spotLight.specular", lightColor * glm::vec3(1.0f));
        objectShader.setFloat("spotLight.constant", 1.0f);
        objectShader.setFloat("spotLight.linear", 0.022f);
        objectShader.setFloat("spotLight.quadratic", 0.0010f);
        objectShader.setFloat("spotLight.innerCone", cosf(glm::radians(15.0f)));
        objectShader.setFloat("spotLight.outerCone", cosf(glm::radians(20.0f)));

        ambientMap.useTextureUnit(0);
        specularMap.useTextureUnit(1);
        if (gEmission) emissionMap.useTextureUnit(2);

        // Draw cubes
        glm::vec3 cubePositions[5] = {
                glm::vec3(0.0f,  0.5f, 0.0f),
                glm::vec3(2.0f, 0.5f, 2.0f),
                glm::vec3(2.0f, 0.5f, -2.0f),
                glm::vec3(-2.0f, 0.5f, -2.0f),
                glm::vec3(-2.0f, 0.5, 2.0f),
        };
        glBindVertexArray(cubeVAO);
        for (int i = 0; i < 5; ++i) {
            // Compute model transformations for each cube
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            objectShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw the ground
        glBindVertexArray(planeVAO);
        glm::mat4 planeModel = glm::mat4(1.0f);
        planeModel = glm::scale(planeModel, glm::vec3(10.0f));
        objectShader.setMat4("model", planeModel);
        groundTexture.useTextureUnit(0);
        // Use 0 to set the active texture to default texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Rendering Ends here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *init()
{
    // Initialization of GLFW context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For the window to run in Mac OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // For anti-aliasing effects
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "Lighting Scene", nullptr, nullptr);
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
    glViewport(0, 0, gScreenWidth * 2, gScreenHeight * 2);

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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        gFunky = !gFunky;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        gEmission = !gEmission;
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
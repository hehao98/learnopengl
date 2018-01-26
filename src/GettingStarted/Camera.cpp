//
// Draw boxes on the screen.
// We can control camera movement using WASD.
//
// Created by Hao He on 18-1-22.
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
#include "Texture.h"
#include "Camera.h"

int gScreenWidth = 800;
int gScreenHeight = 600;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

// The Option to set the visibility of the smiling face
float gVisibility = 0.0f;

Camera gCamera;

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

    gCamera.Position = glm::vec3(0.0f, 0.0f, 3.0f);

    // Load textures
    Texture containerTexture("textures/container.jpg");
    Texture faceTexture("textures/awesomeface.png");

    // Load shaders
    Shader shader("shaders/Box.vert", "shaders/TexturedTriangle.frag");
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // A set of vertices to describe a cube
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Vertex Buffer Objects(VBO) are used to pass vertices to GPU for the vertex shader
    unsigned int VBO;
    // 1 is assigned as the unique ID to this VBO
    glGenBuffers(1, &VBO);

    // Vertex Array Objects(VAO) are used to store vertex attribute pointers
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Tell OpenGL how tp interpret vertex data
    // Pass data to layout(location=0), each data 3 values, type float, no normalization,
    // with the stride as 3*sizeof(float)
    // location 0: vertex location
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // location 1: texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate how much time since last frame
        auto currentFrame = (float)glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // Handle user input
        processInput(window);

        // All the rendering stars from here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up view and projection matrix
        glm::mat4 view = gCamera.GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(gCamera.Zoom),
                                      (float)gScreenWidth / gScreenHeight, 0.1f, 100.0f);

        shader.use();
        shader.setFloat("visibility", gVisibility);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        containerTexture.useTextureUnit(0);
        faceTexture.useTextureUnit(1);

        glm::vec3 cubePositions[] = {
                glm::vec3( 0.0f,  0.0f,  0.0f),
                glm::vec3( 2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3( 2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3( 1.3f, -2.0f, -2.5f),
                glm::vec3( 1.5f,  2.0f, -2.5f),
                glm::vec3( 1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; ++i) {
            // Compute model transformations for each cube
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * (i + 1),
                                glm::vec3(sinf(i + 1), sinf(i + 1), sinf(i + 1)));

            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "box", nullptr, nullptr);
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

    // Adjust the smile face
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (gVisibility < 1.0f) {
            gVisibility += 0.01f;
            gVisibility = std::min(1.0f, gVisibility);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (gVisibility > 0.0f) {
            gVisibility -= 0.01f;
            gVisibility = std::max(0.0f, gVisibility);
        }
    }

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
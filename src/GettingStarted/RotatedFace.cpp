//
// Draw a Rotated face on the screen.
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

// The Option to set the visibility of the smiling face
float visibility = 0.2f;

// Perform necessary initialization.
// Returns pointer to a initialized window with OpenGL context set up
GLFWwindow *init();
// Sometimes user might resize the window. so the OpenGL viewport should be adjusted as well.
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// User input is handled in this function
void processInput(GLFWwindow *window);

int main()
{
    GLFWwindow *window = init();
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW and OpenGL!" << std::endl;
        return -1;
    }

    // Load textures
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: textures/container.jpg" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: textures/aewsomeface.png" << std::endl;
    }
    stbi_image_free(data);

    // Load shaders
    Shader shader("shaders/RotatedFace.vert", "shaders/TexturedTriangle.frag");
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // A set of vertices to describe a rectangle
    float vertices[] = {
            // Positions        // colors         // texture coordinates
            -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    };
    unsigned int indices[] = {
            0, 1, 2, // first triangle
            0, 2, 3, // second triangle
    };

    // Vertex Buffer Objects(VBO) are used to pass vertices to GPU for the vertex shader
    unsigned int VBO;
    // 1 is assigned as the unique ID to this VBO
    glGenBuffers(1, &VBO);

    // Vertex Array Objects(VAO) are used to store vertex attribute pointers
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Element Buffer Objects(EBO) describe additional information like indices of triangles
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Tell OpenGL how tp interpret vertex data
    // Pass data to layout(location=0), each data 3 values, type float, no normalization,
    // with the stride as 3*sizeof(float)
    // location 0: vertex location
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // location 1: vertex color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // location 2: texture coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Handle user input
        processInput(window);

        // All the rendering is done here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto t = (float)glfwGetTime();

        // Draw first container
        shader.use();
        shader.setFloat("visibility", visibility);
        glm::mat4 trans = glm::mat4(1.0f);

        trans = glm::rotate(trans, t, glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Draw second container
        trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(sinf(t), sinf(t), sinf(t)));
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);



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
    GLFWwindow *window = glfwCreateWindow(800, 600, "container", nullptr, nullptr);
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
    glViewport(0, 0, 800, 600);

    // Set the windows resize callback function
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    return window;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (visibility < 1.0f) {
            visibility += 0.01f;
            visibility = std::min(1.0f, visibility);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (visibility > 0.0f) {
            visibility -= 0.01f;
            visibility = std::max(0.0f, visibility);
        }
    }
}


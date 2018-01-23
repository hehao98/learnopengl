//
// Draw 2 triangles with two VBOs, meaning they represent two different graphic objects
// This time they are of different color
//
// Created by Hao He on 18-1-22.
//

#include <iostream>

// GLAD: A library that wraps OpenGL functions to make things easier
//       Note that GLAD MUST be included before GLFW
#include "glad/glad.h"
// GLFW: A library that helps us manage windows
#include <GLFW/glfw3.h>

// Load necessary config information from CMake
#include "config.h"

const char *vertexShaderSource = "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";
const char *fragmentShaderSource = "#version 330 core\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}";
const char *fragmentShaderSource2 = "#version 330 core\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}";

// Sometimes user might resize the window. so the OpenGL viewport should be adjusted as well.
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
// User input is handled in this function
void processInput(GLFWwindow *window);

int main()
{
    int success;
    char infoLog[512];

    // Initialization of GLFW context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell OpenGL the size of rendering window
    glViewport(0, 0, 800, 600);

    // Set the windows resize callback function
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Failed to compile vertex shader" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Failed to compile fragment shader" << infoLog << std::endl;
    }

    unsigned int yellowFragmentShader;
    yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &fragmentShaderSource2, nullptr);
    glCompileShader(yellowFragmentShader);
    glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(yellowFragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Failed to compile fragment shader" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR: Failed to link shader program" << infoLog << std::endl;
    }

    unsigned int yellowShader = glCreateProgram();
    glAttachShader(yellowShader, vertexShader);
    glAttachShader(yellowShader, yellowFragmentShader);
    glLinkProgram(yellowShader);
    glGetProgramiv(yellowShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(yellowShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Failed to link shader program" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(yellowFragmentShader);

    // A set of vertices to describe a triangle
    float triangle1[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
    };
    // Vertex Buffer Objects(VBO) are used to pass vertices to GPU for the vertex shader
    unsigned int triangle1VBO;
    // 1 is assigned as the unique ID to this VBO
    glGenBuffers(1, &triangle1VBO);
    // Vertex Array Objects(VAO) are used to store vertex attribute pointers
    unsigned int triangle1VAO;
    glGenVertexArrays(1, &triangle1VAO);
    glBindVertexArray(triangle1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle1VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    // Tell OpenGL how tp interpret vertex data
    // Pass data to layout(location=0), each data 3 values, type float, no normalization,
    // with the stride as 3*sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    float triangle2[] = {
            -0.5f, -0.75f, 0.0f,
            -1.0f, -0.75f, 0.0f,
            -0.75f, 0.0f, 0.0f,
    };
    unsigned int triangle2VBO;
    glGenBuffers(1, &triangle2VBO);
    unsigned int triangle2VAO;
    glGenVertexArrays(1, &triangle2VAO);
    glBindVertexArray(triangle2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangle2VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Handle user input
        processInput(window);

        // All the rendering is done here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Object1
        glBindVertexArray(triangle1VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(yellowShader);

        // Object2
        glBindVertexArray(triangle2VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate triangle1
    glDeleteVertexArrays(1, &triangle1VAO);
    glDeleteBuffers(1, &triangle1VBO);
    // Deallocate triangle2
    glDeleteVertexArrays(1, &triangle2VAO);
    glDeleteBuffers(1, &triangle2VBO);

    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


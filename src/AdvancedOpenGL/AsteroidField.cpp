//
// Created by 何昊 on 27/02/2018.
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
#include "Model.h"

int gScreenWidth = 800;
int gScreenHeight = 600;

float gDeltaTime = 0.0f;
float gLastFrame = 0.0f;

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
// Generate a cube map using the 6 file paths in the vector,
// Sequence: Right, left, top, bottom, back, front
unsigned int generateCubeMap(std::vector<std::string> facePaths);

int main()
{
    GLFWwindow *window = init();
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW and OpenGL!" << std::endl;
        return -1;
    }

    // Load shaders
    Shader objectShader("shaders/MultipleLights.vert", "shaders/MultipleLights.frag");
    objectShader.use();
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    objectShader.setInt("material.emission", 2);

    Shader instanceShader("shaders/AsteroidField.vert", "shaders/MultipleLights.frag");
    instanceShader.use();
    instanceShader.setInt("material.diffuse", 0);
    instanceShader.setInt("material.specular", 1);
    instanceShader.setInt("material.emission", 2);

    Shader skyboxShader("shaders/SkyboxShader.vert", "shaders/SkyboxShader.frag");

    std::vector<std::string> skyboxPaths = {
            "textures/mp_vr/vr_lf.tga",
            "textures/mp_vr/vr_rt.tga",
            "textures/mp_vr/vr_up.tga",
            "textures/mp_vr/vr_dn.tga",
            "textures/mp_vr/vr_ft.tga",
            "textures/mp_vr/vr_bk.tga",
    };
    unsigned int skyboxTexture = generateCubeMap(skyboxPaths);

    Model planetModel("models/planet/planet.obj");
    Model asteroidModel("models/rock/rock.obj");

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
    unsigned int skyBoxVBO, skyBoxVAO;
    glGenBuffers(1, &skyBoxVBO);
    glGenVertexArrays(1, &skyBoxVAO);
    glBindVertexArray(skyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    gCamera.Position = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 lightSource = glm::vec3(8.2f, 0.5f, 1.0f);
    glm::vec3 lightColor = glm::vec3(1.0f);

    // Initialize asteroid field data
    srand((unsigned int)time(nullptr));
    const int amount = 500;
    glm::mat4 asteroidMatrices[amount];
    std::vector<glm::vec3> asteroidPos;
    std::vector<glm::vec3> asteroidRotationAxis;
    std::vector<float> asteroidAngleOffset;
    std::vector<glm::vec3> asteroidScale;

    for (int i = 0; i < amount; ++i) {
        float r = 10.0f, x = ((rand() % 2000) / 100.0f) - 10.0f;
        glm::vec3 pos = glm::vec3(x, 0, pow(-1, i) * pow(r*r - x*x, 0.5));
        pos += glm::vec3(rand()%100/100.0f,rand()%100/100.0f,rand()%100/100.0f);
        asteroidPos.push_back(pos);
        asteroidRotationAxis.emplace_back(rand()%100/100.0f,rand()%100/100.0f,rand()%100/100.0f);
        asteroidAngleOffset.push_back(rand() % 9000 / 100.0f);
        asteroidScale.emplace_back((rand()%300)/1800.0f);

        // Caculate a unique model matrix for every asteroid
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, asteroidPos[i]);
        model = glm::rotate(model, glm::radians(asteroidAngleOffset[i]),
                            asteroidRotationAxis[i]);
        model = glm::scale(model, asteroidScale[i]);
        asteroidMatrices[i] = model;
    }
    // Load the model data into OpenGL
    unsigned int modelBuffer;
    glGenBuffers(1, &modelBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &asteroidMatrices[0], GL_STATIC_DRAW);
    for (int i = 0; i < asteroidModel.meshes.size(); ++i) {
        unsigned int VAO = asteroidModel.meshes[i].VAO;
        glBindVertexArray(VAO);
        GLsizei vec4sz = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4sz, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4sz, (void*)(vec4sz));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4sz, (void*)(vec4sz*2));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4sz, (void*)(vec4sz*3));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate how much time since last frame
        auto currentFrame = (float)glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // Handle user input
        processInput(window);

        // Set up view and projection matrix
        glm::mat4 view = gCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom),
                                                (float)gScreenWidth / gScreenHeight, 0.1f, 100.0f);

        // All the rendering starts from here
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw skybox first
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        skyboxShader.use();
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setInt("skybox", 0);
        glDepthMask(GL_FALSE);
        glBindVertexArray(skyBoxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        objectShader.use();
        glm::vec3 spotLightTarget = glm::vec3(1.5f*cosf((float)glfwGetTime()), 0.0f,
                                              1.5f*sinf((float)glfwGetTime()));

        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);
        objectShader.setVec3("viewPos", gCamera.Position);

        // Set up material properties
        objectShader.setFloat("material.shininess", 32.0f);

        objectShader.setVec3("light.position", lightSource);
        objectShader.setVec3("light.ambient", lightColor * glm::vec3(0.3f));
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

        glm::mat4 model = glm::mat4(1.0f);
        objectShader.setMat4("model", model);

        planetModel.Draw(objectShader);

        instanceShader.use();
        instanceShader.setMat4("view", view);
        instanceShader.setMat4("projection", projection);
        instanceShader.setVec3("viewPos", gCamera.Position);

        // Set up material properties
        instanceShader.setFloat("material.shininess", 32.0f);

        instanceShader.setVec3("light.position", lightSource);
        instanceShader.setVec3("light.ambient", lightColor * glm::vec3(0.3f));
        instanceShader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));
        instanceShader.setVec3("light.specular", lightColor * glm::vec3(1.0f));
        instanceShader.setFloat("light.constant", 1.0f);
        instanceShader.setFloat("light.linear", 0.022f);
        instanceShader.setFloat("light.quadratic", 0.0010f);

        instanceShader.setVec3("dirLight.direction", glm::vec3(-1.0f, -1.0f, 0.0f));
        instanceShader.setVec3("dirLight.ambient", lightColor * glm::vec3(0.05f));
        instanceShader.setVec3("dirLight.diffuse", lightColor * glm::vec3(0.3f));
        instanceShader.setVec3("dirLight.specular", lightColor * glm::vec3(1.0f));

        instanceShader.setVec3("spotLight.position", glm::vec3(0.0f, 3.0f, 0.0f));
        instanceShader.setVec3("spotLight.direction", spotLightTarget - glm::vec3(0.0f, 3.0f, 0.0f));
        instanceShader.setVec3("spotLight.ambient", lightColor * glm::vec3(0.1f));
        instanceShader.setVec3("spotLight.diffuse", lightColor * glm::vec3(0.5f));
        instanceShader.setVec3("spotLight.specular", lightColor * glm::vec3(1.0f));
        instanceShader.setFloat("spotLight.constant", 1.0f);
        instanceShader.setFloat("spotLight.linear", 0.022f);
        instanceShader.setFloat("spotLight.quadratic", 0.0010f);
        instanceShader.setFloat("spotLight.innerCone", cosf(glm::radians(15.0f)));
        instanceShader.setFloat("spotLight.outerCone", cosf(glm::radians(20.0f)));

        for(unsigned int i = 0; i < asteroidModel.meshes.size(); i++)
        {
            glBindVertexArray(asteroidModel.meshes[i].VAO);
            glDrawElementsInstanced(
                    GL_TRIANGLES, asteroidModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
            );
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
    // Fix Mac OS crash bug
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "Asteroid Field", nullptr, nullptr);
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
    glViewport(0, 0, 2 * gScreenWidth, 2 * gScreenHeight);

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
#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utilities.hpp"
#include "Shader.hpp"
#include <vector>
#include <array>

GLuint CreateVertexBuffer(unsigned int location, unsigned int size, std::vector<GLfloat> data) {
    GLuint id = 0;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(location);
    return id;
}

GLuint CreateIndexBuffer(std::vector<GLuint> data) {
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(),
                 GL_STATIC_DRAW);
    return id;
}

int main(int, char*[]) {

    // Initialize OpenGL

    glfwInit();

    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(vidmode->height / 2, vidmode->height / 2, "GLprimer", nullptr, nullptr);

    if (!window) {
        std::cout << "Unable to open window. Terminating.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(0);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glEnable(GL_CULL_FACE);

    /// Create VAO and VBO

    const std::vector<GLfloat> coords = {
        -1.0f, 1.0f,  1.0f,  // Top 1
         1.0f, 1.0f,  1.0f,  // Top 2
         1.0f, 1.0f, -1.0f,  // Top 3
        -1.0f, 1.0f, -1.0f,  // Top 4

        -1.0f, -1.0f,  1.0f, // Bottom 1
         1.0f, -1.0f,  1.0f, // Bottom 2
         1.0f, -1.0f, -1.0f, // Bottom 3
        -1.0f, -1.0f, -1.0f, // Bottom 4
    };

    const std::vector<GLfloat> colors = {
        1.0f, 0.0f, 0.0f, // Top 1
        1.0f, 0.0f, 0.0f, // Top 2
        1.0f, 0.0f, 0.0f, // Top 3
        1.0f, 0.0f, 0.0f, // Top 4

        0.0f, 0.0f, 1.0f, // Bottom 1
        0.0f, 0.0f, 1.0f, // Bottom 2
        0.0f, 0.0f, 1.0f, // Bottom 3
        0.0f, 0.0f, 1.0f, // Bottom 4
    };

    const std::vector<GLuint> indicies = {
        1, 0, 4, // Front 1
        1, 4, 5, // Front 2

        2, 1, 5, // Right 1
        2, 5, 6, // Right 2

        3, 2, 6, // Back 1
        3, 6, 7, // Back 2

        7, 0, 3, // Left 1
        7, 4, 0, // Left 2

        0, 1, 2, // Top 1
        0, 2, 3, // Top 2

        7, 5, 4, // Bottom 1 
        7, 6, 5, // Bottom 2
    };

    GLuint vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    GLuint vertexBufferID = CreateVertexBuffer(0, 3, coords);
    GLuint colorBufferID = CreateVertexBuffer(1, 3, colors);
    GLuint indexBufferID = CreateIndexBuffer(indicies);

    Shader myShader = Shader("vertex.glsl", "fragment.glsl");
    //Shader myShader = Shader("/home/linn/Documents/VSCode/TNM046/lab2/GLprimer/vertex.glsl", "/home/linn/Documents/VSCode/TNM046/lab2/GLprimer/fragment.glsl");

    GLint locationTime = glGetUniformLocation(myShader.id(), "time");
    GLint locationT = glGetUniformLocation(myShader.id(), "translation");

    std::array<GLfloat, 16> matT = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(myShader.id());
        glBindVertexArray(vertexArrayID);

        glUniform1f(locationTime, glfwGetTime());
        glUniformMatrix4fv(locationT, 1, GL_TRUE, matT.data());

        glDrawElements(GL_TRIANGLES, std::size(indicies), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &indexBufferID);
    glDeleteBuffers(1, &colorBufferID);

    glfwDestroyWindow(window);
    glfwTerminate();
}
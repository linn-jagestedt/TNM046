#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utilities.hpp"
#include "TriangleSoup.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <vector>
#include <array>

int main(int, char*[]) {

    // Initialize OpenGL

    glfwInit();

    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(
            600, 
            600, 
            "lab2", 
            nullptr, 
            nullptr
        );

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
    glEnable(GL_DEPTH_TEST);

    Shader myShader = Shader("/home/linn/Documents/VSCode/TNM046/lab5/GLprimer/vertex.glsl", "/home/linn/Documents/VSCode/TNM046/lab5/GLprimer/fragment.glsl");

    GLint locationTime = glGetUniformLocation(myShader.id(), "time");
    GLint locationV = glGetUniformLocation(myShader.id(), "view");
    GLint locationP = glGetUniformLocation(myShader.id(), "proj");

    /// Create Teapot

    TriangleSoup teapot;
    teapot.readOBJ("/home/linn/Documents/VSCode/TNM046/lab5/GLprimer/meshes/teapot.obj");
    Texture teapotTexture = Texture("/home/linn/Documents/VSCode/TNM046/lab5/GLprimer/textures/pingu.tga");

    /// Create Sphere

    TriangleSoup sphere;
    sphere.createSphere(0.42, 100);
    Texture sphereTexture = Texture("/home/linn/Documents/VSCode/TNM046/lab5/GLprimer/textures/earth.tga");

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) 
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(myShader.id());
        
        GLfloat time = glfwGetTime();
        glUniform1f(locationTime, time);

        std::array<GLfloat, 16> matP = 
            util::mat4perspective(
                M_PI / 4.0, 
                1.0f, 
                0.1f, 
                100.0f
            );

        std::array<GLfloat, 16> TeapotMatV = {
            0.021, 0,   0,   0,
            0,   0.021, 0,   0,
            0,   0,   0.021, 0,
            0.2,   -0.1,   -3.3, 1.0
        };

        std::array<GLfloat, 16> SphereMatV = util::multiplyMatrices(
            {
                1.0, 0,   0,   0,
                0,   1.0, 0,   0,
                0,   0,   1.0, 0,
                0,   0,   -3.0, 1.0
            },
            util::getYRotation(time)
        );
        
        glUniformMatrix4fv(locationP, 1, GL_FALSE, matP.data());

        glUniformMatrix4fv(locationV, 1, GL_FALSE, TeapotMatV.data());
        glBindTexture(GL_TEXTURE_2D, teapotTexture.id());
        teapot.render();

        glUniformMatrix4fv(locationV, 1, GL_FALSE, SphereMatV.data());
        glBindTexture(GL_TEXTURE_2D, sphereTexture.id());
        sphere.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
}
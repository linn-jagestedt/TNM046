/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#include "Utilities.hpp"

#include <cstdio>
#include <iostream>
#include <math.h>
#include <string.h>

namespace util 
{
    double displayFPS(GLFWwindow* window) {
        static int frames = 0;
        static double fps = 0.0;

        static double t0 = glfwGetTime();  // Gets number of seconds since glfwInit()

        double t = glfwGetTime();  // Get current time

        // update fps only once every second
        if (t - t0 >= 1.0) {
            fps = static_cast<double>(frames) / (t - t0);
            t0 = t;
            frames = 0;
        }

        // update the window title
        if (frames == 0) {
            char title[201];
            // convert fps to milliseconds
            double frametime = (fps > 0.0) ? 1000.0 / fps : 0.0;
            snprintf(title, 200, "TNM046: %.2f ms/frame (%.1f FPS)", frametime, fps);
            glfwSetWindowTitle(window, title);
        }

        ++frames;
        return fps;
    }

    std::array<GLfloat, 16> transpose(std::array<GLfloat, 16> a) {
        std::array<GLfloat, 16> result;
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result[(i * 4) + j] = a[i + (j * 4)];
            }
        }

        return result;
    }

    std::array<GLfloat, 16> multiplyMatrices(std::array<GLfloat, 16> a, std::array<GLfloat, 16> b) 
    {
        std::array<GLfloat, 16> result;
        memset(result.data(), 0, sizeof(GLfloat) * 16);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result[i + (j * 4)] += a[(k * 4) + i] * b[(j * 4) + k];
                }
            }
        }

        return result;
    }

    std::array<GLfloat, 16> getYRotation(GLfloat angle) { 
        return transpose({
            cos(angle), 0, sin(angle), 0,
            0, 1, 0, 0,
            -sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1
        });
    }

    std::array<GLfloat, 16> getZRotation(GLfloat angle) { 
        return transpose({
            cos(angle), -sin(angle), 0, 0,
            sin(angle), cos(angle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        });
    }

    std::array<GLfloat, 16> getXRotation(GLfloat angle) { 
        return transpose({
            1, 0, 0, 0,
            0, cos(angle), -sin(angle), 0,
            0, sin(angle), cos(angle), 0,
            0, 0, 0, 1
        });
    }

    std::array<float, 16> mat4perspective(float vfov, float aspect, float znear, float zfar) {
        float f = tan(M_PI_2 - (vfov / 2.0f));
        float M11 = f/aspect;
        float M22 = f;
        float M34 = -1;
        float M33 = -(zfar + znear) / (zfar - znear);
        float M43 = -(2 * zfar * znear) / (zfar - znear);
        return {
            M11, 0, 0, 0,
            0, M22, 0, 0,
            0, 0, M33, M34,
            0, 0, M43, 0
        };
    }
}
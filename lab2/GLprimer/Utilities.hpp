/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#pragma once

#include <array>
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace util {

/*
 * displayFPS() - Calculate, display and return frame rate statistics.
 * Called every frame, but statistics are updated only once per second.
 * The time per frame is a better measure of performance than the
 * number of frames per second, so both are displayed.
 *
 * NOTE: This function doesn't work properly if you call it for multiple
 * windows. Call it only for one window, and only once every frame.
 */
double displayFPS(GLFWwindow* window);
std::array<GLfloat, 16> multiplyMatrices(std::array<GLfloat, 16> a, std::array<GLfloat, 16> b);
std::array<GLfloat, 16> getYRotation(GLfloat angle);
std::array<GLfloat, 16> getZRotation(GLfloat angle);
std::array<GLfloat, 16> getXRotation(GLfloat angle);
std::array<GLfloat, 16> transpose(std::array<GLfloat, 16> a);

}  // namespace util

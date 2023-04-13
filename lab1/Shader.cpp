/*
 * GLSL Shader loading and compilation
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

#include <iostream>
#include <fstream>

Shader::Shader() : programID_(0) {}

Shader::Shader(const std::string& vertexshaderfile, const std::string& fragmentshaderfile) {
    createShader(vertexshaderfile, fragmentshaderfile);
}

Shader::~Shader() {
    if (programID_ != 0) {
        glDeleteProgram(programID_); 
    }
}

GLuint Shader::id() const { return programID_; }

std::string readFile(const std::string& filename) 
{
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        std::cerr << "Error: Could not open shader file '" << filename << "'\n";
        return {};
    }

    in.seekg(0, std::ios_base::end);
    size_t fileLength = in.tellg();
    in.seekg(0); 

    std::string buffer;
    buffer.resize(fileLength + 1);
    in.read(buffer.data(), fileLength);
    if (in.bad()) {
        std::cerr << "Error: Could not read shader file '" << filename << "'\n";
        return {};
    }

    buffer[fileLength] = '\0';
    in.close();

    return buffer;
}

GLuint loadShader(GLenum shaderType, const std::string& filename) 
{
    GLuint shader = glCreateShader(shaderType);
    std::string shaderSource = readFile(filename);

    if (!shaderSource.empty()) {
        const char* source = shaderSource.c_str();
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
    }

    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE) {
        static char buf[4096] = {0}; 
        glGetShaderInfoLog(shader, sizeof(buf), nullptr, buf);
        std::cerr << "Shader compile error ('" << filename << "'):\n" << buf << "\n";
    }

    return shader;
}

void Shader::createShader(const std::string& vertexshaderfile, const std::string& fragmentshaderfile) 
{
    if (programID_ != 0) {
        glDeleteProgram(programID_);
    }

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexshaderfile);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentshaderfile);

    GLuint programObject = glCreateProgram();
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);

    GLint shadersLinked = GL_FALSE;
    glGetProgramiv(programObject, GL_LINK_STATUS, &shadersLinked);

    if (shadersLinked == GL_FALSE) {
        char buf[4096] = {0};
        glGetProgramInfoLog(programObject, sizeof(buf), nullptr, buf);
        std::cerr << "Shader program linker error:\n" << buf << "\n";
    }

    glDeleteShader(vertexShader);  
    glDeleteShader(fragmentShader);

    programID_ = programObject; 
}

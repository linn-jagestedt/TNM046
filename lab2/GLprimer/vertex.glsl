#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

uniform float time;
uniform mat4 translation;

out vec3 interpolatedColor;

void main() {

    mat4 matrisZ = mat4(
        cos(time), -sin(time), 0, 0,
        sin(time), cos(time), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    mat4 matrisY = mat4(
        cos(time / 3), 0, sin(time / 3), 0,
        0, 1, 0, 0,
        -sin(time / 3), 0, cos(time / 3), 0,
        0, 0, 0, 1
    );

    gl_Position = translation * matrisY * matrisZ * vec4(Position / 2, 1.0);
    interpolatedColor = Color;
}
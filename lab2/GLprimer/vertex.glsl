#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

uniform float time;
uniform mat4 translation;

out vec3 uColor;
out float uTime;

void main() {
    gl_Position =  translation * vec4(Position / 8, 1.0);
    uColor = Color;
    uTime = time;
}
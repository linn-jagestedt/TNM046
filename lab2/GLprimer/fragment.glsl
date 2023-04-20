#version 330 core

in vec3 uColor;
in float uTime;

out vec4 finalcolor;

void main() {
    finalcolor = vec4(uColor.r, (sin(uTime) + 1) / 2, uColor.b, 1);
}
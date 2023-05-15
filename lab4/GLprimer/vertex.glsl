#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

uniform float time;
uniform mat4 view;
uniform mat4 proj;

out float uTime;
out vec3 uNormal;
out vec2 UV;

void main() {
	gl_Position =  proj * view  * vec4(Position, 1.0);
	uNormal = normalize(mat3(view) * Normal);
	UV = TexCoord;
    uTime = time;
}
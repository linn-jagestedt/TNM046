#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

uniform float time;
uniform mat4 translation;


out mat4 uTranslation;
out float uTime;
out vec3 uNormal;
out vec2 UV;

void main() {
	gl_Position = vec4(Position, 1.0);
	uNormal = Normal;
	UV = TexCoord;
    uTime = time;
    uTranslation = translation;
}
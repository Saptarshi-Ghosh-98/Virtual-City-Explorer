#version 440 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 UV_model;

out vec2 UV;

uniform mat4 MVP;

void main() {

	gl_Position =  MVP * vec4(vertexPos, 1);
	UV = UV_model;
}
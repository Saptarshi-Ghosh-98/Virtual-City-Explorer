#version 440 core

layout(location = 0) in vec3 vertexPos_model;

uniform mat4 MVP;

void main() {

	gl_Position =  MVP * vec4(vertexPos_model, 1);
	
}


#version 440

layout(location = 0) in vec3 vertex;

out vec3 UV;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vertex, 1.0);
	UV = vertex;
}
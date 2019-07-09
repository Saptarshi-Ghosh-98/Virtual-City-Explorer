#version 440

in vec3 UV;

out vec4 color;

uniform samplerCube texSampler;

void main() {
	color = texture(texSampler, UV);
}
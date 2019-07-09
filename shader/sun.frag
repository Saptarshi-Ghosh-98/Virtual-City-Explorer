#version 440 core

in vec2 UV;
out vec4 color;

uniform sampler2D flareTex;
uniform float brightness;

void main() {
	color = texture(flareTex, UV).rgba;
	color.a *= brightness;
}
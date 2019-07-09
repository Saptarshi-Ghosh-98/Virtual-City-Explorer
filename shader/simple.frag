#version 440 core

in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform sampler2D shadowTex;

void main() {
	color = texture(myTextureSampler, UV).rgb * texture(shadowTex, UV/10.0).rgb;
}
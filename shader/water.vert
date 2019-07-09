#version 440 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 UV_pos;

out vec2 UV;
out vec3 normal;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 M;

uniform float time;

void main() {
	mat4 trans;
	float a = UV_pos.x - 0.5;
	float b = UV_pos.y - 0.5;

	trans[0] = vec4(1, 0, 0, 0);
	trans[1] = vec4(0, 1, 0, 0);
	trans[2] = vec4(0, 0, 1, 0);
	trans[3] = vec4(0, 0, sin((a*b)*20 + time/100)/5, 1);

	gl_Position = MVP * (trans * vec4(vertexPos, 1));

	normal = normalize(vec3(0, 1, cos((a*b)*20 + time/100)/5));	//OUT
	fragPos = vec3(M * trans * vec4(vertexPos, 1));
	UV = UV_pos;
}

/*

#version 440 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 UV_pos;

out vec2 UV;
out vec3 normal;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 M;

uniform float time;

void main() {
	mat4 trans;
	trans[0] = vec4(1, 0, 0, 0);
	trans[1] = vec4(0, 1, 0, 0);
	trans[2] = vec4(0, 0, 1, 0);
	trans[3] = vec4(0, 0, sin((UV_pos.y*UV_pos.x)*20 + time/100)/5, 1);

	gl_Position = MVP * (trans * vec4(vertexPos, 1));

	normal = normalize(vec3(0, 1, cos((UV_pos.y*UV_pos.x)*20 + time/100)/5));	//OUT
	fragPos = vec3(M * trans * vec4(vertexPos, 1));
	UV = UV_pos;
}
*/
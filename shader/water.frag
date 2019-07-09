#version 440 core

in vec2 UV;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform sampler2D waterTex;
uniform float alpha;
uniform vec3 viewPos;

void main() {

	vec3 lightPos = vec3(40, 30, 5.5);
	//vec3 lightPos = vec3(lightPos4.x/lightPos4.w, lightPos4.y/lightPos4.w, lightPos4.z/lightPos4.w);
	vec3 lightColor = vec3(1,1,1);

	vec3 waterColor = vec3(111/255., 159/255., 237/255.);//vec3(0.25,0.525,0.956);

	// vec3(0.25,0.525,0.956);//
	//Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = clamp(dot(norm, lightDir), 0.0, 1.0);
	vec3 diffuse = diff * lightColor;

	//Specular
	float specularStrength = 5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 200);
	vec3 specular = specularStrength * spec * lightColor;

	color = vec4((ambient+diffuse)*waterColor + specular*vec3(1, 1, 0.5), 1.0);

	// color = vec4((ambient+diffuse)*waterColor + specular*vec3(0.9, 0.9, 1.0), 1.0);
	//
	color *= texture(waterTex, UV);
	color.a *= alpha;

}
#pragma once

#include "object.h"
#include <array>
#include <vector>
#include <tuple>

extern GLuint VertexArrayID[];
extern glm::mat4 proj, view;
extern std::array<GLuint, 10> shaderID;

class Terrain : public Object {
	public:

	glm::mat4 model;
	glm::mat4 mvp;

	int imgWidth, imgHeight;
	int repeat;
	float scaleMap, scaleHeight;

	GLuint programID;
	GLuint Texture;
	GLuint uni_MVP, uni_TextureSampler;
	
	Terrain(const char * heightImageFile);
	void load(const char * imageFile, GLuint shader_ID);
	void drawObject(GLuint Tex);

	std::tuple<int,int,int,float> getSize();
};
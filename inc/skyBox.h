#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <iostream>
#include "texture.h"

extern glm::mat4 proj, view;
extern std::array<GLuint, 10> shaderID;
extern int screenWidth, screenHeight;
extern glm::vec3 PlayerPos, PlayerDir;

class skyBox {
	public:

	std::vector<float> skyVertex;
	
	GLuint VAO;
	GLuint VBO;

	GLuint programID, TexID;

	skyBox();
	void load(const char *imagepath, GLuint shaderID);
	void draw();
	~skyBox();
};
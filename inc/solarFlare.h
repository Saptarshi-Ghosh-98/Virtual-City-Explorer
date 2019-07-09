#pragma once

#include "object.h"
#include "texture.h"
#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

extern glm::mat4 proj, view;
extern glm::vec3 PlayerPos, PlayerDir;
extern int screenWidth, screenHeight;
extern double mouseX, mouseY;

class solarFlare : public Object {

	int programID;
	std::vector<const char *> Textures_Image;
	std::vector<GLuint> TexID;
	std::vector<GLuint> Texture;
	std::vector<float> FlareScale;
	float FlareDistance;

	glm::mat4 model, mvp;

	GLuint uni_MVP;
	GLuint uni_TexSampler;
	GLuint uni_brightness;

	public:
	
	solarFlare();
	void load(const char *modelFileName, GLuint shader_ID);
	void drawFlare();
	~solarFlare();

};
#pragma once

#include "object.h"
#include "texture.h"
#include "shader_utils.h"

extern glm::mat4 proj, view, proj_ortho;
extern std::array<GLuint, 10> shaderID;
extern glm::vec3 PlayerDir;

class drawableObject : public Object {

	glm::mat4 model;
	glm::mat4 mvp;

	GLuint programID;
	GLuint uni_MVP, uni_M, uni_V, uni_TextureSampler, LightID, texEnID;

	GLuint Texture;

	std::vector<glm::vec3> Instance_worldPos;

  public:
	drawableObject();
	drawableObject(std::vector<glm::vec3> instance_position);

	void load(const char *modelFileName, const char *textureFileName, GLuint shader_ID, GLint channels = 3);
	void load(GLuint shader_ID);
	void drawObject(glm::mat4 transform, glm::vec3 worldPos);
	void drawObjectInstance(glm::mat4 transform);
	void drawObjectShadowInstance(glm::mat4 transform, glm::mat4 VP, GLint uni_shadow_MVP);
	void drawObjectShadow(glm::mat4 transform, glm::vec3 WorldPos, glm::mat4 VP, GLint uni_shadow_MVP);


	
	void drawAxis();
	void drawSky();
	void drawFlare();
	~drawableObject();
};
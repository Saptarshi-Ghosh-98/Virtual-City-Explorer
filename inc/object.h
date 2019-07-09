#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>

extern GLuint VertexArrayID[];

class Object {

	public:
	
	static int VAO_Index;
	int VAO_index_obj;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normal;

	GLuint VBO[3];

	Object();
	Object(const char *modelFileName);
	void load(const char *modelFileName);
	void generateVBO();
	void drawObject();
	~Object();

};
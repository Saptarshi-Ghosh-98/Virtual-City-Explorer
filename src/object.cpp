#include "object.h"
#include "obj_loader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <thread>
#include <array>

extern GLuint VertexArrayID[];

int Object::VAO_Index = 0;

Object::Object() {
	VAO_index_obj = VAO_Index++;
}

Object::Object(const char *modelFileName) {
	loadObject(modelFileName, vertex, uv, normal);
	VAO_index_obj = VAO_Index++;
	generateVBO();
}

void Object::load(const char *modelFileName) {
	loadObject(modelFileName, vertex, uv, normal);
	generateVBO();
}

void Object::generateVBO() {
	glGenVertexArrays(1, &VertexArrayID[VAO_index_obj]);
	glBindVertexArray(VertexArrayID[VAO_index_obj]);
	
	glGenBuffers(1, &VBO[0]);			// Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO[1]);			// UV
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &VBO[2]);			// Normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	glEnableVertexAttribArray(2);
}

Object::~Object() {
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteBuffers(1, &VBO[2]);
	glDeleteVertexArrays(1, &VertexArrayID[VAO_index_obj]);
}

void Object::drawObject() {
	// Set up the MVP Matrices and ProgramID Before Calling the Function
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
}
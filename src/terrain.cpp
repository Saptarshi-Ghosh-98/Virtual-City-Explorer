#include "object.h"
#include "terrain.h"
#include <ctime>

Terrain::Terrain() {

	srand(time(0));
	
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			HeightMap[i][j] = 0;
		}
	}
	glm::vec3 normalxy[9][9];

	//Terrain Generation
	for (int i = 1; i < 8; ++i) {
		for (int j = 1; j < 8; ++j) {
			
			glm::vec3 v = glm::vec3(i, j, HeightMap[i][j]);
			glm::vec3 v_n = glm::vec3(i, j+1, HeightMap[i][j+1]);
			glm::vec3 v_e = glm::vec3(i+1, j, HeightMap[i+1][j]);
			glm::vec3 v_s = glm::vec3(i, j-1, HeightMap[i][j-1]);
			glm::vec3 v_w = glm::vec3(i-1, j, HeightMap[i-1][j]);

			vertex.insert(vertex.end(),  {v, v_e, v_n});
			vertex.insert(vertex.end(),  {v, v_s, v_e});
			vertex.insert(vertex.end(),  {v, v_n, v_w});
			vertex.insert(vertex.end(),  {v, v_w, v_s});

			glm::vec3 n[4];
			n[0] = glm::cross(v_e - v, v_n - v);
			n[1] = glm::cross(v_s - v, v_e - v);
			n[2] = glm::cross(v_n - v, v_w - v);
			n[3] = glm::cross(v_w - v, v_s - v);
			glm::vec3 norm = glm::normalize(n[0] + n[1] + n[2] + n[3]);
			normalxy[i][j] = norm;
		}
	}

	for (int i = 1; i < 8; ++i) {
		for (int j = 1; j < 8; ++j) {
			normal.insert(normal.end(), {normalxy[i][j], normalxy[i+1][j], normalxy[i][j+1]});
			normal.insert(normal.end(), {normalxy[i][j], normalxy[i][j-1], normalxy[i+1][j]});
			normal.insert(normal.end(), {normalxy[i][j], normalxy[i][j+1], normalxy[i-1][j]});
			normal.insert(normal.end(), {normalxy[i][j], normalxy[i-1][j], normalxy[i][j-1]});
		}
	}

	glGenVertexArrays(1, &VertexArrayID[VAO_index_obj]);
	glBindVertexArray(VertexArrayID[VAO_index_obj]);
	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), &normal[0], GL_STATIC_DRAW);
}
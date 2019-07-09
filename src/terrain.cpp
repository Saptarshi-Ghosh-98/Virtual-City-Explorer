#include "drawableObject.h"
#include "terrain.h"
#include <ctime>
#include <iostream>
#include <GL/glew.h>
#include "texture.h"

extern glm::mat4 proj, view;
extern std::array<GLuint, 10> shaderID;

Terrain::Terrain(const char * heightImageFile) {

	srand(time(0));

	imgWidth = 0;
	imgHeight = 0;
	unsigned char *HeightImage;
	scaleMap = 0.1;
	scaleHeight = 2;

	std::vector<std::vector<float>> HeightMap;
	std::vector<std::vector<glm::vec3>> NormalMap;


	HeightImage = loadHeightMap(heightImageFile, imgWidth, imgHeight);
	HeightMap.resize(imgWidth, std::vector<float>(imgHeight, 0));
	NormalMap.resize(imgWidth, std::vector<glm::vec3>(imgHeight, glm::vec3(0.0f, 0.0f, 0.0f)));

	// XY Plane Terrain (Orient to XZ Plane before Rendering)
	if (HeightImage != nullptr) {
		for (int j = 0; j < imgHeight; ++j) {
			for (int i = 0; i < imgWidth; ++i) {
				HeightMap[i][j] = scaleHeight * ((int)HeightImage[j*imgWidth + i] / 255.0f);
			}
		}
	}

	SOIL_free_image_data(HeightImage);

	// Normals
	for (int j = 0; j < imgHeight; ++j) {
		for (int i = 0; i < imgWidth; ++i) {
			if (i == 0 || j == 0 || i == imgWidth-1 || j == imgHeight-1) {
				NormalMap[i][j] = (glm::vec3(0.0f, 0.0f, 1.0f));
			} else {
				NormalMap[i][j] = (glm::normalize(glm::vec3(
					(HeightMap[i-1][j] - HeightMap[i+1][j]) / scaleHeight,
					(HeightMap[i][j-1] - HeightMap[i][j+1]) / scaleHeight,
					2.0f
				)));
			}
		}
	}

	int uvWidth = (imgWidth-1);
	int uvHeight = (imgHeight-1);
	repeat = 10;

	//Terrain Generation [UP : (0,0,1)]
	for (int j = 0; j < imgHeight-1; ++j) {
		for (int i = 0; i < imgWidth; ++i) {
			vertex.push_back(glm::vec3(i, j, HeightMap[i][j]));
			vertex.push_back(glm::vec3(i, j+1, HeightMap[i][j+1]));

			normal.push_back(NormalMap[i][j]);
			normal.push_back(NormalMap[i][j+1]);

			uv.push_back(glm::vec2((float)(i*repeat)/(uvWidth), (float)(j*repeat)/(uvHeight))); //
			uv.push_back(glm::vec2((float)(i*repeat)/(uvWidth), (float)((j+1)*repeat)/(uvHeight))); //
		}
		vertex.push_back(glm::vec3(imgWidth-1, j+1, HeightMap[imgWidth-1][j+1]));
		vertex.push_back(glm::vec3(0, j+1, HeightMap[0][j+1]));

		normal.push_back(NormalMap[imgWidth-1][j+1]);
		normal.push_back(NormalMap[0][j+1]);

		uv.push_back(glm::vec2(repeat, (float)((j+1)*repeat)/(uvHeight)));
		uv.push_back(glm::vec2(0, (float)((j+1)*repeat)/(uvHeight)));			
	}
}

void Terrain::load(const char * imageFile, GLuint shader_ID) {
	Object::generateVBO();
	Texture = loadImage(imageFile);

	programID = shader_ID;

	uni_MVP     = glGetUniformLocation(programID, "MVP");
	uni_TextureSampler = glGetUniformLocation(programID, "myTextureSampler");

	glm::mat4 t1 = glm::translate(glm::mat4(1.0f), glm::vec3(-imgWidth/2.0f, -imgHeight/2.0f, 0.0f));
	glm::mat4 t2 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	glm::mat4 t3 = glm::scale(glm::mat4(1.0f), glm::vec3(scaleMap, 1.0f, scaleMap));
	model =  t3 * t2 * t1 * glm::mat4(1.0f);
}

void Terrain::drawObject(GLuint Tex) {
	glUseProgram(programID);
	
	mvp = proj * view * model;

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Tex);
	glUniform1i(uni_TextureSampler, 1);
	glUniform1i(glGetUniformLocation(programID, "shadowTex"), 2);

	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->vertex.size());
}

std::tuple<int,int,int,float> Terrain::getSize() {
	return {imgWidth, imgHeight, 10, scaleMap};
}
#include "water.h"

extern glm::mat4 proj, view;
extern glm::vec3 PlayerPos, PlayerDir;
extern int screenWidth, screenHeight;
extern double mouseX, mouseY;

float timer = 0;

water::water() {
	programID = 0;
	TexID = 0;
}

void water::load(const char *modelFileName, const char *textureFileName, GLuint shader_ID) {
	Object::load(modelFileName);
	
	TexID = loadImage_A(textureFileName);

	programID = shader_ID;
	//Get all Uniforms
	uni_MVP = glGetUniformLocation(programID, "MVP");
	uni_TexSampler = glGetUniformLocation(programID, "waterTex");
	uni_alpha = glGetUniformLocation(programID, "alpha");
}

void water::drawWater() {
	timer++;

	if (timer > 1000000) {
		timer = 0;
	}

	glm::mat t1 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 5.5f));
	glm::mat s1 = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 1.0f));
	glm::mat r1 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = t1*r1*s1;
	mvp = proj * view * model;

	// Brightness
	float alpha = 1.0f - (glm::dot(-PlayerDir, glm::vec3(0.0f, 1.0f, 0.0f)))*0.8f;
	alpha *= 0.5f;
	
	glEnable(GL_BLEND);
	glUseProgram(programID);
	glUniform1f(uni_alpha, alpha);
	glUniform1f(glGetUniformLocation(programID, "time"), (float)timer);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"),1, GL_FALSE, &model[0][0]);
	glUniform3fv(glGetUniformLocation(programID, "viewPos"), 1, &PlayerPos[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glUniform1i(uni_TexSampler, 0);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
	glDisable(GL_BLEND);
}

water::~water() {
	glDeleteTextures(1, &TexID);
}

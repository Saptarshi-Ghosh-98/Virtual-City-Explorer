#include "drawableObject.h"

#include <cstring>

extern glm::mat4 proj, view;
extern std::array<GLuint, 10> shaderID;
extern int screenWidth, screenHeight;
extern glm::vec3 PlayerPos, PlayerDir;

drawableObject::drawableObject() {
	model = glm::mat4(1.0f);
	mvp = glm::mat4(1.0f);
	programID = 0;
	Texture = 0;
}

drawableObject::drawableObject(std::vector<glm::vec3> Instance_POS) {
	model = glm::mat4(1.0f);
	mvp = glm::mat4(1.0f);
	programID = 0;
	Texture = 0;

	this->Instance_worldPos = Instance_POS;
}

void drawableObject::load(const char *modelFileName, const char *textureFileName, GLuint shader_ID, GLint channels) {
	Object::load(modelFileName);

	int len = std::strlen(textureFileName);
	const char *EXT = &textureFileName[len-3];

	if (std::strcmp(EXT, "dds") == 0) {
		Texture   = loadDDS(textureFileName);
	} else {
		if (channels == 4)
			Texture = loadImage_A(textureFileName);
		else
			Texture = loadImage(textureFileName);
	}

	programID = shader_ID;
	uni_MVP   = glGetUniformLocation(programID, "MVP");
	uni_M     = glGetUniformLocation(programID, "M");
	uni_V     = glGetUniformLocation(programID, "V");
	texEnID   = glGetUniformLocation(programID, "texEn");

	uni_TextureSampler = glGetUniformLocation(programID, "myTextureSampler");
	LightID            = glGetUniformLocation(programID, "LightPosition_worldspace");
}

void drawableObject::load(GLuint shader_ID) {
	Object::generateVBO();
	Texture   = 0;

	programID = shader_ID;
	uni_MVP   = glGetUniformLocation(programID, "MVP");
	uni_M     = glGetUniformLocation(programID, "M");
	uni_V     = glGetUniformLocation(programID, "V");
	texEnID   = glGetUniformLocation(programID, "texEn");

	uni_TextureSampler = glGetUniformLocation(programID, "myTextureSampler");
	LightID            = glGetUniformLocation(programID, "LightPosition_worldspace");
}

void drawableObject::drawObject(glm::mat4 transform, glm::vec3 worldPos) {
	glUseProgram(programID);

	model = glm::translate(glm::mat4(1.0f), worldPos) * transform;
	mvp =  proj * view * model;

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(uni_M, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uni_V, 1, GL_FALSE, &view[0][0]);

	glm::vec3 lightPos = glm::vec3(0.0f, 7.0f, 0.0f);
	glUniform3fv(LightID, 1, &lightPos[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(uni_TextureSampler, 0);

	glUniform1i(texEnID, (bool)Texture);
	Object::drawObject();
}

void drawableObject::drawObjectInstance(glm::mat4 transform) {
	glUseProgram(programID);

	glUniformMatrix4fv(uni_V, 1, GL_FALSE, &view[0][0]);

	glm::vec3 lightPos = glm::vec3(0.0f, 7.0f, 0.0f);
	glUniform3fv(LightID, 1, &lightPos[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(uni_TextureSampler, 0);

	glUniform1i(texEnID, (bool)Texture);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);
	
	for (int i = 0; i < Instance_worldPos.size(); ++i) {
		model = glm::translate(glm::mat4(1.0f), Instance_worldPos[i]) * transform;
		mvp =  proj * view * model;

		glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(uni_M, 1, GL_FALSE, &model[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
	}
}

void drawableObject::drawObjectShadowInstance(glm::mat4 transform, glm::mat4 VP, GLint uni_shadow_MVP) {
	glm::mat4 model_shadow(1.0f), mvp_shadow(1.0f);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);
	
	for (int i = 0; i < Instance_worldPos.size(); ++i) {
		model_shadow = glm::translate(glm::mat4(1.0f), Instance_worldPos[i]) * transform;
		mvp_shadow =  VP * model_shadow;

		glUniformMatrix4fv(uni_shadow_MVP, 1, GL_FALSE, &mvp_shadow[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
	}
}

void drawableObject::drawObjectShadow(glm::mat4 transform, glm::vec3 WorldPos, glm::mat4 VP, GLint uni_shadow_MVP) {
	glm::mat4 model_shadow(1.0f), mvp_shadow(1.0f);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);
	
		model_shadow = glm::translate(glm::mat4(1.0f), WorldPos) * transform;
		mvp_shadow =  VP * model_shadow;

		glUniformMatrix4fv(uni_shadow_MVP, 1, GL_FALSE, &mvp_shadow[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
}

drawableObject::~drawableObject() {
	if (Texture > 0)
		glDeleteTextures(1, &Texture);
}

// Coordinate Axis
void drawableObject::drawAxis() {
	glUseProgram(programID);
	//proj_ortho = glm::ortho(0, 2, 0, 2, -5, 100);
	glm::vec3 trans(0.0f, 0.0f, 0.0f);
	glm::mat4 view_axis = glm::lookAt(
								trans - PlayerDir, 
								glm::vec3(0.0, 0.0, 0.0),
								glm::vec3(0.0f, 1.0f, 0.0f));

	// X Axis
	glm::vec3 color(1.0f, 0.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(programID, "Axis_Color"), 1, &color[0]);

	glm::mat4 axis_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.9f, 0.7f, -1.0f));
	glm::mat4 axis_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05,.05,.05));
	glm::mat4 axis_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.f, -1.f));
	model = axis_rotate * axis_scale;
	mvp = axis_translate * proj * view_axis * model;
	
	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
	Object::drawObject();

	// Y Axis
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(programID, "Axis_Color"), 1, &color[0]);
	axis_rotate = glm::mat4(1.0f);
	model = axis_rotate * axis_scale;
	mvp = axis_translate * proj * view_axis * model;

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
	Object::drawObject();

	// Z Axis
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(programID, "Axis_Color"), 1, &color[0]);

	axis_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = axis_rotate * axis_scale;
	mvp = axis_translate * proj * view_axis * model;

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);
	Object::drawObject();

}

// Sky : Sun
void drawableObject::drawSky() {
	glUseProgram(programID);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 0.0f));
	mvp =  proj * view * model;
	
	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(uni_TextureSampler, 0);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
}

void drawableObject::drawFlare() {
	glUseProgram(programID);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 0.0f));
	glm::vec4 sun_pos =  proj * view * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	sun_pos = glm::vec4(sun_pos.x/sun_pos.w, sun_pos.y/sun_pos.w, sun_pos.z/sun_pos.w, 1);

	if (!(sun_pos.x >= -1 && sun_pos.x <= 1 && sun_pos.y >= -1 && sun_pos.y <= 1)) {
		return;
	}
	
	glm::vec4 look_pos = proj * view * glm::vec4(PlayerPos+PlayerDir, 1.0f);
	look_pos = glm::vec4(look_pos.x/look_pos.w, look_pos.y/look_pos.w, look_pos.z/look_pos.w, 1);

	float alpha = 1;
	glm::vec4 flare_pos = (alpha)*(sun_pos) + (1.0f-alpha)*(look_pos);
	glm::vec2 pixel_pos = glm::vec2(((flare_pos.x + 1.0f)/2.0f)*screenWidth, ((flare_pos.y + 1.0f)/2.0f)*screenHeight);
	glm::mat4 t1 = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f, 300.0f, 1.0f));
	glm::mat4 t2 = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_pos, 0.0f));
	model = t2 * t1;
	glm::mat4 p = glm::ortho(0.0, screenWidth*1.0, 0.0, screenHeight*1.0);
	mvp = p * model;

	glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(uni_TextureSampler, 0);

	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
}
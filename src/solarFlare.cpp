#include "solarFlare.h"

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

solarFlare::solarFlare() {

	Textures_Image = 
		{
			"res/sun.png",
			"res/tex1.png",
			"res/tex2.png",
			"res/tex3.png",
			"res/tex4.png",
			"res/tex5.png",
			"res/tex6.png",
			"res/tex7.png",
			"res/tex8.png",
			"res/tex9.png"
		};
	
	FlareScale = {
		1.0f, // Sun
		0.7f,
		0.6f,
		0.6f,
		0.4f,
		0.2f,
		0.12f,
		0.14f,
		0.24f,
		0.6f,
		0.44f,
		0.8f,
		1.0f
	};
	
	FlareDistance = 0.3f;
}

void solarFlare::load(const char *modelFileName, GLuint shader_ID) {
	Object::load(modelFileName);
	
	programID = shader_ID;

	TexID.resize(Textures_Image.size(), 0);

	for (int i = 0; i < Textures_Image.size(); ++i) {
		TexID[i] = loadImage_A(Textures_Image[i]);
	}

	Texture = {
			TexID[0],	// Sun
			TexID[6],
			TexID[4],
			TexID[2],
			TexID[7],
			TexID[1],
			TexID[3],
			TexID[5],
			TexID[7],
			TexID[9],
			TexID[5],
			TexID[4],
			TexID[8]
		};

	uni_MVP = glGetUniformLocation(programID, "MVP");
	uni_TexSampler = glGetUniformLocation(programID, "flareTex");
	uni_brightness = glGetUniformLocation(programID, "brightness");
}

void solarFlare::drawFlare() {
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 0.0f));
	glm::vec4 sun_coord =  proj * glm::mat4(glm::mat3(view)) * glm::vec4(30.0f, 30.0f, 0.0f, 1.0f);

	if (sun_coord.z/sun_coord.w < 0.0 || sun_coord.z/sun_coord.w > 1.0) {
		return;
	}

	glm::vec2 sun_pos = glm::vec2(
							sun_coord.x/sun_coord.w, 
							sun_coord.y/sun_coord.w
						);

	glm::vec2 look_pos(0.0f, 0.0f); 	//Center

	if (!(sun_pos.x >= -1.0f && sun_pos.x <= 1.0f && sun_pos.y >= -1.0f && sun_pos.y <= 1.0f)) {
		return;
	}
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glUseProgram(programID);
	glBindVertexArray(VertexArrayID[this->VAO_index_obj]);

	glm::vec2 flareDir = look_pos - sun_pos;

	/*** Sun ***/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float brightness = 1.0f;
	
	glUniform1f(uni_brightness, brightness);

	for (int i = 0; i < 1; ++i) {
		glm::vec2 flare_pos = sun_pos + (i * FlareDistance)*(flareDir);
		glm::vec2 flare_pixel_pos = glm::vec2(
				((flare_pos.x + 1.0f)/2.0f)*screenWidth,
				((flare_pos.y + 1.0f)/2.0f)*screenHeight
		);

		glm::mat4 tt = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
		glm::mat4 t1 = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f*FlareScale[i], 300.0f*FlareScale[i], 1.0f));
		glm::mat4 t2 = glm::translate(glm::mat4(1.0f), glm::vec3(flare_pixel_pos, 0.0f));
		model = t2 * t1 * tt;
		glm::mat4 p = glm::ortho(0.0, screenWidth*1.0, 0.0, screenHeight*1.0);
		mvp = p * model;

		glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture[i]);
		glUniform1i(uni_TexSampler, 0);

		glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
	}

	/*** Flares ***/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	brightness = 1 - (glm::length(flareDir) / 0.9f);
	
	if (brightness >= 0) {
		glUniform1f(uni_brightness, brightness);

		for (int i = 1; i < Texture.size(); ++i) {
			// Texture i
			// Line : sun_pos + (alpha)(flareDir);
			//
			glm::vec2 flare_pos = sun_pos + ((i-1) * FlareDistance)*(flareDir);
			glm::vec2 flare_pixel_pos = glm::vec2(
					((flare_pos.x + 1.0f)/2.0f)*screenWidth,
					((flare_pos.y + 1.0f)/2.0f)*screenHeight
			);

			glm::mat4 tt = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
			glm::mat4 t1 = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f*FlareScale[i], 300.0f*FlareScale[i], 1.0f));
			glm::mat4 t2 = glm::translate(glm::mat4(1.0f), glm::vec3(flare_pixel_pos, 0.0f));
			model = t2 * t1 * tt;
			glm::mat4 p = glm::ortho(0.0, screenWidth*1.0, 0.0, screenHeight*1.0);
			mvp = p * model;

			glUniformMatrix4fv(uni_MVP, 1, GL_FALSE, &mvp[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture[i]);
			glUniform1i(uni_TexSampler, 0);

			glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

solarFlare::~solarFlare() {
	for (int i = 0; i < TexID.size(); ++i) {
		glDeleteTextures(1, &TexID[i]);
	}
}
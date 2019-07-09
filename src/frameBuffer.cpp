#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <tuple>
#include <GLFW/glfw3.h>

#include "drawableObject.h"
#include "shader_utils.h"

extern drawableObject tree1, house1, lamp1, rock, eagle;
extern int screenWidth, screenHeight;

GLuint FBO;
GLuint texture;

void create_FBO() {
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

GLuint render_ShadowFBO(std::tuple<int,int,int,float> Terrain_Info, GLuint programID) {

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glBindTexture(GL_TEXTURE_2D, texture);

	#define Terrain_Width 	std::get<0>(Terrain_Info)
	#define Terrain_Height 	std::get<1>(Terrain_Info)
	#define Terrain_Repeat	std::get<2>(Terrain_Info)
	#define Terrain_Scale	std::get<3>(Terrain_Info)

	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA, 
				 	Terrain_Width*Terrain_Repeat,
				 	Terrain_Height*Terrain_Repeat, 
				 	0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FrameBuffer Incomplete" << std::endl;
		return 0;
	}

	glViewport(0, 0, Terrain_Width*Terrain_Repeat, Terrain_Height*Terrain_Repeat);

	glm::mat4 proj_shadow = glm::ortho( -Terrain_Width*Terrain_Scale/2.0, +Terrain_Width*Terrain_Scale/2.0, 
										-Terrain_Height*Terrain_Scale/2.0, +Terrain_Height*Terrain_Scale/2.0, 
										-5.0, 100.0);
	
	glm::mat4 view_shadow = glm::lookAt(
		glm::vec3(1.5, 30., 0.),	// Eye
		glm::vec3(0., 0.0 , 0.),	// Look At
		glm::vec3(0., 0. , -1.)		// Up
	);

	glm::mat4 VP_shadow = proj_shadow * view_shadow;

	glUseProgram(programID);
	GLint uni_shadow_MVP = glGetUniformLocation(programID, "MVP");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 tr = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
	glm::mat4 tr1 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));

	// Shadow - Draw
		tree1.drawObjectShadowInstance(glm::mat4(1.0f), VP_shadow, uni_shadow_MVP);
		house1.drawObjectShadowInstance(m, VP_shadow,uni_shadow_MVP);
		house1.drawObjectShadowInstance(tr*r*m, VP_shadow,uni_shadow_MVP);
		lamp1.drawObjectShadowInstance(glm::mat4(1.0f), VP_shadow, uni_shadow_MVP);
		rock.drawObjectShadowInstance(glm::mat4(1.0f), VP_shadow, uni_shadow_MVP);
		//house1.drawObjectShadow(m, glm::vec3(5.0f, 1.0f, -16.0f), VP_shadow, uni_shadow_MVP);
		//eagle.drawObjectShadow(glm::rotate(glm::mat4(1.0f),(float)glfwGetTime()/4,glm::vec3(0.0f,1.0f,0.0f))*
	 	//glm::translate(glm::vec3(-20.0f,0.0f,-20.0f)), glm::vec3(-10.0f, 2.0f, 0.0f), VP_shadow, uni_shadow_MVP);
	// Shadow - Draw End

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDeleteFramebuffers(1, &FBO);

	glClearColor(196/255.0f, 218/255.0f, 255/255.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,screenWidth,screenHeight);

	#undef Terrain_Width
	#undef Terrain_Height
	#undef Terrain_Repeat
	#undef Terrain_Scale

	return texture;
}

void deleteFBO() {
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &texture);
}
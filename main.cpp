#include "global.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#include <array>

GLFWwindow *window;
GLFWmonitor *monitor;

bool wireframe = 0;
bool fullscreen = 0;

float ratio;
int screenWidth, screenHeight;
double mouseX = 100, mouseY = 100;

GLuint VertexArrayID[2];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = 0;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = 1;
		}

	} else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		exit(0);

	} else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (fullscreen == 0) {
			glfwSetWindowMonitor(window, monitor, 0, 0, glfwGetVideoMode(monitor)->width, glfwGetVideoMode(monitor)->height, glfwGetVideoMode(monitor)->refreshRate);
			fullscreen = 1;
		} else {
			glfwSetWindowMonitor(window, NULL, 100, 100, 1024, 768, glfwGetVideoMode(monitor)->refreshRate);
			fullscreen = 0;
		}
	}
}

void mouseCallback(GLFWwindow* window, double x, double y) {
	mouseX = x;
	mouseY = y;
}

void resizeCallback(GLFWwindow* window, int width, int height) 
{
	screenWidth  = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
	ratio = (float)width/height;
}

void print(glm::mat4 v) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << v[j][i] << " ";
		}
		std::cout << std::endl;
	}
}

#ifndef NVIDIA
#if defined(_WIN32)
extern "C" 
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
#endif
#endif

void initializeGLFW() {
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(1366, 768, "Virtual City Explorer", NULL, NULL);
	ratio = float(1366)/768;
	screenWidth = 1366;
	screenHeight = 768;

	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, 1366/2, 768/2);

	mouseX = 1366/2;
	mouseY = 768/2;
}


int main() {

	initializeGLFW();

	glClearColor(0.5, 0.5, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);

	// Objects
	Object tree("model/tree.obj");
	Terrain ground;

	//Shader
	GLuint programID = LoadShaders("shader/t.glsl.vert", "shader/f.glsl.frag");
	GLuint uniMVP = glGetUniformLocation(programID, "MVP");
	GLuint uniM = glGetUniformLocation(programID, "M");
	GLuint uniV = glGetUniformLocation(programID, "V");

	GLuint Texture = 0;
	GLuint TexID = glGetUniformLocation(programID, "myTextureSampler");

	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	
	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	double prevTime = glfwGetTime();

	int frame_count = 0;
	char title[50];

	glfwPollEvents();

	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		double currentTime = glfwGetTime();
		frame_count++;

	 	if (currentTime - prevTime >= 1.0) {
			sprintf(title, "Virtual City Explorer : %d FPS", frame_count);
			glfwSetWindowTitle(window, title);
			frame_count = 0;
			prevTime += 1.0;
		}

		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 proj = glm::perspective(45.0f, ratio, 0.01f, 100.00f);
		glm::mat4 view = glm::lookAt(
							glm::vec3(0.0f, 2.0f, 10.0f),
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = proj * view * model;
	
		glUniformMatrix4fv(uniMVP, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(uniM, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniV, 1, GL_FALSE, &view[0][0]);
	
	
		glm::vec3 lightPos = glm::vec3(2.0, 3.0, 1.2);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TexID, 0);

		// Object 1 : Tree
		tree.drawObject();

		// Ground
		glm::mat4 t1 = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -4.0f, 0.0f));
		glm::mat4 t2 = glm::rotate(glm::mat4(1.0f), glm::radians(85.f), glm::vec3(-1.0f,0.0f,0.0f));
		model = t2 * (t1 * model);
		mvp = proj * view * model;
	
		glUniformMatrix4fv(uniMVP, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(uniM, 1, GL_FALSE, &model[0][0]);

		ground.drawObject();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	
	glfwTerminate();
	return 0;
}
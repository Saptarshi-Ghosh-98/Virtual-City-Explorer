/****************************************************************************************************************************
 *                                                                                                                          *
 *     ___      ___ ___  ________  _________  ___  ___  ________  ___               ________  ___  _________    ___    ___  *
 *    |\  \    /  /|\  \|\   __  \|\___   ___\\  \|\  \|\   __  \|\  \             |\   ____\|\  \|\___   ___\ |\  \  /  /| *
 *    \ \  \  /  / | \  \ \  \|\  \|___ \  \_\ \  \\\  \ \  \|\  \ \  \            \ \  \___|\ \  \|___ \  \_| \ \  \/  / / *
 *     \ \  \/  / / \ \  \ \   _  _\   \ \  \ \ \  \\\  \ \   __  \ \  \            \ \  \    \ \  \   \ \  \   \ \    / /  *
 *      \ \    / /   \ \  \ \  \\  \|   \ \  \ \ \  \\\  \ \  \ \  \ \  \____        \ \  \____\ \  \   \ \  \   \/  /  /   *
 *       \ \__/ /     \ \__\ \__\\ _\    \ \__\ \ \_______\ \__\ \__\ \_______\       \ \_______\ \__\   \ \__\__/  / /     *
 *        \|__|/       \|__|\|__|\|__|    \|__|  \|_______|\|__|\|__|\|_______|        \|_______|\|__|    \|__|\___/ /      *
 *                                                                                                            \|___|/       *
 *                                                                                                                          *
 *                                                                                                                          *
 *                 _______      ___    ___ ________  ___       ________  ________  _______   ________                       *
 *                |\  ___ \    |\  \  /  /|\   __  \|\  \     |\   __  \|\   __  \|\  ___ \ |\   __  \                      *
 *                \ \   __/|   \ \  \/  / | \  \|\  \ \  \    \ \  \|\  \ \  \|\  \ \   __/|\ \  \|\  \                     *
 *                 \ \  \_|/__  \ \    / / \ \   ____\ \  \    \ \  \\\  \ \   _  _\ \  \_|/_\ \   _  _\                    *
 *                  \ \  \_|\ \  /     \/   \ \  \___|\ \  \____\ \  \\\  \ \  \\  \\ \  \_|\ \ \  \\  \|                   *
 *                   \ \_______\/  /\   \    \ \__\    \ \_______\ \_______\ \__\\ _\\ \_______\ \__\\ _\                   *
 *                    \|_______/__/ /\ __\    \|__|     \|_______|\|_______|\|__|\|__|\|_______|\|__|\|__|                  *
 *                             |__|/ \|__|                                                                                  *
 *                                                                                                                          *
 ****************************************************************************************************************************
 *                                                                                                                          *
 *   ____              _                 _     _      ____ _               _                                                *
 *  / ___|  __ _ _ __ | |_ __ _ _ __ ___| |__ (_)    / ___| |__   ___  ___| |__                                             *
 *  \___ \ / _` | '_ \| __/ _` | '__/ __| '_ \| |   | |  _| '_ \ / _ \/ __| '_ \                                            *
 *   ___) | (_| | |_) | || (_| | |  \__ \ | | | |   | |_| | | | | (_) \__ \ | | |                                           *
 *  |____/ \__,_| .__/ \__\__,_|_|  |___/_| |_|_|    \____|_| |_|\___/|___/_| |_|                                           *
 *              |_|                                                                                                         *
 ****************************************************************************************************************************
 *                                                                                                                          *
 * Author : Saptarshi Ghosh                                                                                                 *
 *			160002052                                                                                                       *
 ****************************************************************************************************************************
 */

#include "global.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/constants.hpp>
#include "texture.h"
#include "render.h"
#include "text_render.h"
#include <SOIL/SOIL.h>

#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#include <array>


//********************
//*     Defines      *
//********************

#define NVIDIA
#define GL_DEBUG
// #define VSYNC
#define PLAYER_RESTRICT

//********************
//*     Globals      *
//********************

GLFWwindow *window;
GLFWmonitor *monitor;

bool wireframe = 0;
bool fullscreen = 0;

float ratio;
int screenWidth, screenHeight;
double mouseX = 100, mouseY = 100;

float PlayerStep = 0.5f;

glm::vec3 PlayerPos(0.0f, 2.0f, 0.0f);
glm::vec3 PlayerDir(0.0f, 0.0f, -1.0f);
const glm::vec3 WorldUp(0.0f, 1.0f, 0.0f);

float theta, phi;

float FOV = 58.31f;

bool showHelp = false;
bool showCardinality = true;

//***********************************
//*     GLFW CallBack Funtions      *
//***********************************

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
			glfwSwapInterval(1);
		} else {
			glfwSetWindowMonitor(window, NULL, 100, 100, 1367, 768, glfwGetVideoMode(monitor)->refreshRate);
			fullscreen = 0;
			glfwSwapInterval(1);
		}
	} else if (key == GLFW_KEY_UP) {
		PlayerPos += PlayerStep * glm::normalize(glm::vec3(PlayerDir.x, 0.0f, PlayerDir.z));
	} else if (key == GLFW_KEY_DOWN) {
		PlayerPos -= PlayerStep * glm::normalize(glm::vec3(PlayerDir.x, 0.0f, PlayerDir.z));
	} else if (key == GLFW_KEY_RIGHT) {
		PlayerPos += PlayerStep * glm::normalize(glm::cross(
									glm::normalize(glm::vec3(PlayerDir.x, 0.0f, PlayerDir.z)),
									WorldUp));
	} else if (key == GLFW_KEY_LEFT) {
		PlayerPos -= PlayerStep * glm::normalize(glm::cross(
									glm::normalize(glm::vec3(PlayerDir.x, 0.0f, PlayerDir.z)),
									WorldUp));
	} else if (key == GLFW_KEY_I) {
		PlayerPos.y += 0.1f;
	} else if (key == GLFW_KEY_K) {
		PlayerPos.y -= 0.1;
	} else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		showHelp = !showHelp;
	} else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		showCardinality = !showCardinality;
	}

	#ifdef PLAYER_RESTRICT
	#define PLAYER_MAX 21.0f
	PlayerPos.x = glm::clamp(PlayerPos.x, -PLAYER_MAX, PLAYER_MAX);
	PlayerPos.z = glm::clamp(PlayerPos.z, -PLAYER_MAX, PLAYER_MAX);
	#undef PLAYER_MAX
	#endif
}

void mouseCallback(GLFWwindow* window, double x, double y) {
	mouseX = x;
	mouseY = y;

	theta = (mouseX * 2) * glm::pi<float>() / screenWidth;
	phi = (mouseY - screenHeight/2) * glm::pi<float>() / (screenHeight);

	PlayerDir = glm::normalize(glm::vec3(glm::cos(theta), -glm::sin(phi), glm::sin(theta)));
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	FOV += yoffset * 0.8;
}

void resizeCallback(GLFWwindow* window, int width, int height) 
{
	screenWidth  = width;
	screenHeight = height;
	glViewport(0, 0, width, height);
	ratio = (float)width/height;
}

#ifdef NVIDIA
#if defined(_WIN32)
extern "C" 
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
#endif
#endif

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar *message, GLvoid *userParam)
{

    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	std::cout << "---------------------------------------" << std::endl;
    std::cout << "Debug : (" << id << ") " << message << std::endl;

	switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}


void initializeGLFW() {

	glfwInit();
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#ifdef GL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
	#endif
	
	monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(1367, 768, "Virtual City Explorer", NULL, NULL);
	ratio = float(1367)/768;
	screenWidth = 1367;
	screenHeight = 768;

	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, 3*1367/4, 768/2);
	glfwSwapInterval(1);

	mouseX = 3*screenWidth/4;
	mouseY = screenHeight/2;
}


int main() {
	
	initializeGLFW();

	glClearColor(196/255.0f, 218/255.0f, 255/255.0f, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	GLFWimage icon;
	icon.pixels = SOIL_load_image("Icon.png", &icon.width, &icon.height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, &icon);
	SOIL_free_image_data(icon.pixels);

	#ifdef GL_DEBUG
	if (glDebugMessageCallback != NULL) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback((GLDEBUGPROC)debugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		std::cout << "GL_KHR_debug Supported" << std::endl;
	}
	#endif

	// Set up Models, Textures etc.
	sceneSetup();

	glfwPollEvents();

	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		getFPS();

		// Main Draw Function
		renderScene();
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	sceneDelete();
	glfwTerminate();
	return 0;
}
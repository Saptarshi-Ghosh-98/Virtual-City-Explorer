#include "global.h"
#include "render.h"
#include "texture.h"
#include "text_render.h"
#include "frameBuffer.h"

#include "solarFlare.h"
#include "water.h"
#include "skyBox.h"

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/constants.hpp>

#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#include <array>

GLuint VertexArrayID[10];

extern GLFWwindow *window;
extern GLFWmonitor *monitor;

extern bool wireframe;
extern bool fullscreen;

extern float ratio;
extern int screenWidth, screenHeight;
extern double mouseX, mouseY;
extern glm::vec3 PlayerPos, PlayerDir;

glm::mat4 proj, view, proj_ortho;

extern float FOV;

extern bool showHelp, showCardinality;

std::array<GLuint, 10> shaderID;

/**** FPS ****/
double prevTime;
int frame_count = 0;
char title[50];
char debug[10];

// Terrain Height : 1.4f
#define Terrain_Height 0.25f
/**** Constant : Positions etc... ****/
const std::vector<glm::vec3> TREE_POS = {
	{0.0f, Terrain_Height, 0.0f},
	{1.0f, Terrain_Height, 0.5f},
	{1.4f, Terrain_Height, 1.0f},
	{-0.5f, Terrain_Height, 0.0f},
	{10.0f, Terrain_Height, -2.f},
	//{0.0f, Terrain_Height, 20.0f},
	{10.0f, Terrain_Height, -3.0f},
	// Auto
	{0.0f, Terrain_Height, 2.5f},
	{0.0f, Terrain_Height, 5.0f},
	{0.0f, Terrain_Height, 7.5f},
	{0.0f, Terrain_Height, 10.0f},
	{0.0f, Terrain_Height, 12.5f},
	{0.0f, Terrain_Height, 15.0f},
	{0.0f, Terrain_Height, 17.5f},
	{0.0f, Terrain_Height, -2.5f},
	{0.0f, Terrain_Height, -5.0f},
	{0.0f, Terrain_Height, -7.5f},
	{0.0f, Terrain_Height, -10.0f},
	{0.0f, Terrain_Height, -12.5f},
	{0.0f, Terrain_Height, -15.0f},
	//{0.0f, Terrain_Height, -17.5f},

	//End Auto
};

const std::vector<glm::vec3> HOUSE_POS = {
	{-5.0f, 1.0f, 5.0f},
	{-5.0f, 1.0f, 8.5f},
	{-5.0f, 1.0f, 12.0f},
	{-5.0f, 1.0f, 15.5f},
	{-5.0f, 1.0f, 19.0f},
	{-5.0f, 1.0f, 1.5f},
	{-5.0f, 1.0f, -2.0f},
	{-5.0f, 1.0f, -5.5f},
	{-5.0f, 1.0f, -9.0f},
	{-5.0f, 1.0f, -12.5f},
	{-5.0f, 1.0f, -16.0f},
	{-5.0f, 1.0f, -19.5f}
};


const std::vector<glm::vec3> AXIS_POS = {
	{8.0f, 1.0f, 8.0f}
};

const std::vector<glm::vec3> WATER_POS = {
	{14.0f, 0.0f, 5.5f}
};

const std::vector<glm::vec3> LAMP_POS = {
	{-10.0, 1.0f, 0.0f},
	{-10.0, 1.0f, 3.0f},
	{-10.0, 1.0f, 6.0f},
	{-10.0, 1.0f, 9.0f},
	{-10.0, 1.0f, 12.0f},
	{-10.0, 1.0f, 15.0f},
	{-10.0, 1.0f, 18.0f},
	{-10.0, 1.0f, -3.0f},
	{-10.0, 1.0f, -6.0f},
	{-10.0, 1.0f, -9.0f},
	{-10.0, 1.0f, -12.0f},
	{-10.0, 1.0f, -15.0f},
	{-10.0, 1.0f, -18.0f}
};

const std::vector<glm::vec3> ROCK_POS = {
	{5.0f, 1.0f, -5.0f},
	{13.5f, 1.0f, -0.5f}
};

const std::vector<glm::vec3> EAGLE_POS = {
	{0.0f, 20.0f, 0.0f}
};
#undef Terrain_Height

/**** Drawable Objects ****/

drawableObject tree1(TREE_POS);
drawableObject house1(HOUSE_POS);
drawableObject axis1(AXIS_POS);
drawableObject waterHolder(WATER_POS);
drawableObject lamp1(LAMP_POS);
drawableObject rock(ROCK_POS);
drawableObject eagle(EAGLE_POS);

solarFlare flare;
water WATER;
Terrain ground("res/terrain.png");
Text text0, text_debug;

skyBox sky;
GLuint Texture_Shadow = 0;


void sceneSetup() {
	proj = glm::perspective(glm::radians<float>(FOV), ratio, 0.01f, 100.00f);
	proj_ortho = glm::ortho(0.0, screenWidth*1., 0.0, screenHeight*1., -5.0, 100.0);
	shaderID.fill(0);
	
	const std::vector<std::pair<const char *, const char *>> shaderList = {
		{"shader/v.glsl.vert", "shader/f.glsl.frag"},
		{"shader/simple.vert", "shader/simple.frag"},
		{"shader/Font_Render.glsl.vert", "shader/Font_Render.glsl.frag"},
		{"shader/shadow.vert", "shader/shadow.frag"},
		{"shader/axis.glsl.vert", "shader/axis.glsl.frag"},
		{"shader/sun.vert", "shader/sun.frag"},
		{"shader/water.vert", "shader/water.frag"},
		{"shader/sky.glsl.vert", "shader/sky.glsl.frag"},
		{"shader/house.glsl.vert", "shader/house.glsl.frag"}
	};

	for (int i = 0; i < shaderList.size(); ++i) {
		shaderID[i] = LoadShaders(shaderList[i].first, shaderList[i].second);
	}

	tree1.load("model/tree.obj", "res/tree_birch_leaf.dds", shaderID[0]);
	axis1.load("model/axis.obj", "res/cottage_normal.png", shaderID[4]);
	flare.load("model/sun_2.obj", shaderID[5]);
	ground.load("res/terrain_texture.png", shaderID[1]);
	text0.initFreeType(shaderID[2], "font/arial.ttf");
	text_debug.initFreeType(shaderID[2], "font/arial.ttf");
	WATER.load("model/water.obj", "res/water.png", shaderID[6]);
	waterHolder.load("model/cube.obj", "res/terrain.png", shaderID[1]);
	sky.load("res/sky", shaderID[7]);
	house1.load("model/house.obj", "res/cottage_diffuse.dds", shaderID[8]);
	lamp1.load("model/lamp.obj", "res/lamp.png", shaderID[8]);
	rock.load("model/rock.obj", "res/rock.png", shaderID[8]);
	eagle.load("model/eagle.obj", "res/eagle.png", shaderID[8]);

	prevTime = glfwGetTime();

	create_FBO();
	Texture_Shadow = render_ShadowFBO(ground.getSize(), shaderID[3]);

}

void renderScene() {

	// view = glm::lookAt(
	// 	//glm::vec3(0.0f, 0.0f, 2.0f),
	// 	glm::vec3(10.0f * glm::sin(-theta), phi, 10.0f * glm::cos(-theta)), // Eye
	// 	glm::vec3(0.0f, 0.0f, 0.0f),										// Look At
	// 	glm::vec3(0.0f, 1.0f, 0.0f));										// Up
	// Texture_Shadow = render_ShadowFBO(ground.getSize(), shaderID[3]);

	proj = glm::perspective(glm::radians<float>(FOV), ratio, 0.01f, 100.00f);

	/**
	 * theta : XZ
	 * phi : Y
	 * 
	 * cos(theta), sin(phi), -sin(theta)
	 */
	view = glm::lookAt(
		PlayerPos,
		PlayerPos + PlayerDir,
		glm::vec3(0.0f,1.0f, 0.0f)
	);
	

	glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0.1));
	glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 tr = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f));
	glm::mat4 tr1 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));

	/* Tree */
	tree1.drawObjectInstance(glm::mat4(1.0f));

	/* House */
	house1.drawObjectInstance(m);
	house1.drawObjectInstance(tr*r*m);
	house1.drawObject(m, glm::vec3(5.0f, 1.0f, -16.0f));
	house1.drawObject(m, glm::vec3(5.0f, 1.0f, -19.5f));
	house1.drawObject(m, glm::vec3(5.0f, 1.0f, 16.0f));
	house1.drawObject(m, glm::vec3(5.0f, 1.0f, 19.5f));
	house1.drawObject(tr1*r*m, glm::vec3(5.0f, 1.0f, -16.0f));
	house1.drawObject(tr1*r*m, glm::vec3(5.0f, 1.0f, -19.5f));
	house1.drawObject(tr1*r*m, glm::vec3(5.0f, 1.0f, 16.0f));
	house1.drawObject(tr1*r*m, glm::vec3(5.0f, 1.0f, 19.5f));

	/* Axis */
	axis1.drawAxis();
	/* Lamp */
	lamp1.drawObjectInstance(glm::mat4(1.0f));

	/* Rock */
	rock.drawObjectInstance(glm::mat4(1.0f));

	/* Eagle */
	eagle.drawObjectInstance(glm::rotate(glm::mat4(1.0f),(float)glfwGetTime()/4,glm::vec3(0.0f,1.0f,0.0f))*
    glm::translate(glm::vec3(-20.0f,0.0f,-20.0f)));
    eagle.drawObjectInstance(glm::rotate(glm::mat4(1.0f),(float)glfwGetTime()/4,glm::vec3(0.0f,1.0f,0.0f))*
    glm::translate(glm::vec3(20.0f,0.0f,20.0f)));


	/* Ground */
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
		
		ground.drawObject(Texture_Shadow);

	glDisable(GL_CULL_FACE);

	/* Water Holder */
	// glm::mat t = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 2.f, 4.0f));
	// waterHolder.drawObjectInstance(t);
	
	// SkyBox
	sky.draw();

	/* Text */
	text0.render("Welcome to Virtual City!", 100, 100, 0.5f, glm::vec4( glm::abs(glm::sin(2.5*glfwGetTime())) , 0.0f, 0.0f, 1.0f));
	text_debug.render(debug, screenWidth - 120, screenHeight-48, 0.5, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	/* Sun + Flares */
	flare.drawFlare();

	/* Water */
	WATER.drawWater();

	/* Help */
	if (showHelp) {
		text0.render(
			" Welcome to Virtual City ! \n\n"
			"1. Arrow Keys - Move Around\n"
			"2. Mouse - Look Around\n"
			"3. Scroll - Change Field of View\n"
			"4. W - Wireframe Toggle\n"
			"5. I - Move Up\n"
			"6. K - Move Down\n"
			"7. D - Show Direction\n"
			"8. H - Toggle Help\n"
			"9. Q - Quit\n", screenWidth/2-100, screenHeight/2+100, 0.5f, glm::vec4(237/255., 30/255., 113/255., 1.0)
		);
	}

	/* Cardinality */
	if (showCardinality) {
		float dirTheta = glm::degrees(glm::acos(glm::dot(PlayerDir, glm::vec3(1.0f, 0.0f, 0.0f))));
		std::string dirStr;
		if (PlayerDir.z < 0.0f) {
			if (dirTheta > 45.0f && dirTheta < 135.0f) {
				dirStr = "N";
			} else if (dirTheta > 135.0f) {
				dirStr = "W";
			} else {
				dirStr = "E";
			}
		} else {
			if (dirTheta > 45.0f && dirTheta < 135.0f) {
				dirStr = "S";
			} else if (dirTheta > 135.0f) {
				dirStr = "W";
			} else {
				dirStr = "E";
			}
		}
		text0.render(dirStr, screenWidth / 2, 0.95*screenHeight, 1.0f, glm::vec4(glm::sin(2.5*glfwGetTime()) , 0.0f, 0.0f, 1.0f));
	}


}

void sceneDelete() {
	deleteFBO();
	for (int i = 0; i < shaderID.size(); ++i) {
		glDeleteProgram(shaderID[i]);
	}
}

void getFPS() {
	double currentTime = glfwGetTime();
	frame_count++;

	if (currentTime - prevTime >= 1.0) {
		sprintf(title, "Virtual City Explorer : %d FPS", frame_count);
		sprintf(debug, "FPS : %d", frame_count);
		glfwSetWindowTitle(window, title);
		frame_count = 0;
		prevTime += 1.0;
	}
}
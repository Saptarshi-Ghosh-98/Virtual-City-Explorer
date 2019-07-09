#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <string>

extern int screenWidth, screenHeight;

class Text {

	GLuint programID;
	GLuint VAO, VBO;

	//Display the Text - Helper
	void renderText_helper(std::string &text, GLfloat &x, GLfloat &y, GLfloat &scale, glm::vec4 &color);

  public:
	
	Text();

	//Initialize FreeType
	void initFreeType(GLuint shaderID, const char *fontName);

	//Display the Text
	void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color);
	
	~Text();
};


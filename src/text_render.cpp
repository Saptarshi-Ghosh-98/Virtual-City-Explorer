#include "text_render.h"

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_GTC_constants
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

extern int screenWidth, screenHeight;

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLint Advance;
};

std::map<GLchar, Character> Characters;



//Display the Text - Helper
void Text::renderText_helper(std::string &text, GLfloat &x, GLfloat &y, GLfloat &scale, glm::vec4 &color) {

	GLfloat x_original = x;

	glUseProgram(programID);
	glUniform4fv(glGetUniformLocation(programID, "textColor"), 1, glm::value_ptr(color));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);


	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {


		Character ch = Characters[*c];

		if (*c == '\n') {
			y -= 48 * scale;
			x = x_original;
			continue;
		} else if (*c == '\t') {
			x += 4 * ((Characters[' '].Advance >> 6) * scale);
			continue;
		}

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },            
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }           
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;

	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Text::Text() {
	programID = 0;
	VAO = 0;
	VBO = 0;
}

void Text::initFreeType(GLuint shaderID, const char *fontName) {
	
	programID = shaderID;

	glm::mat4 projection = glm::ortho(	0.0f, static_cast<GLfloat>(screenWidth), 
										0.0f, static_cast<GLfloat>(screenHeight));
	
	glUseProgram(programID);
	glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(glGetUniformLocation(programID, "text"), 0);
	glActiveTexture(GL_TEXTURE0);
	
	// FreeType
	FT_Library ft;

	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Error : FreeType : Could not Initialize FreeType" << std::endl;
		return;
	}

	FT_Face face;

	if (FT_New_Face(ft, fontName, 0, &face)) {
		std::cerr << "Error : FreeType : Failed to Load Font" << std::endl;
		return;
	}

	std::cerr << "\nFont Loaded : " << fontName << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

	for (GLubyte c = 0; c < 128; c++) {

		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "Error : FreeType : Failed to load Glyph : " << char(c) << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//Display the Text
void Text::render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color) {
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderText_helper(text, x, y, scale, color);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

Text::~Text() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}



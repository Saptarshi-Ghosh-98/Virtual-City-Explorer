#include "skyBox.h"

extern glm::mat4 proj, view;
extern std::array<GLuint, 10> shaderID;
extern int screenWidth, screenHeight;
extern glm::vec3 PlayerPos, PlayerDir;

skyBox::skyBox() {
		skyVertex = {      
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};
	}

	void skyBox::load(const char *imagepath, GLuint shaderID) {
		programID = shaderID;
		TexID = loadCubeMap(imagepath);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, skyVertex.size() * sizeof(float), &skyVertex[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	void skyBox::draw() {
		glDepthMask(GL_FALSE);
		glUseProgram(programID);
		glm::mat4 MVP = proj * glm::mat4(glm::mat3(view)) * glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f));

		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TexID);
		glUniform1i(glGetUniformLocation(programID, "texSampler"), 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
	}


	skyBox::~skyBox() {
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glDeleteTextures(1, &TexID);
	}
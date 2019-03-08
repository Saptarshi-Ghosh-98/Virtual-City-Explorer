#include "shader_utils.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <gl/glew.h>

GLuint LoadShaders(const char *vertexShader, const char *fragmentShader) {

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::stringstream ss;

	std::string vertexShaderCode;
	std::ifstream vertexfile(vertexShader, std::ios::in);

	std::cerr << "\nLoading Shaders : " << vertexShader << " ";
	if (vertexfile.is_open()) {
		ss << vertexfile.rdbuf();
		vertexShaderCode = ss.str();
		vertexfile.close();
	} else {
		std::cerr << "| Cannot open File \n";
		return 0;
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentFile(fragmentShader, std::ios::in);

	std::cerr << "\nLoading Shaders : " << fragmentShader << " ";
	if (fragmentFile.is_open()) {
		ss.str(std::string());
		ss << fragmentFile.rdbuf();
		fragmentShaderCode = ss.str();
		fragmentFile.close();
	} else {
		std::cerr << "| Cannot open File \n";
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	//Compile Vertex Shader
	std::cerr << "\nCompiling Shader : " << vertexShader << " \n";
	char const *vertexCodeptr = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexCodeptr, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		std::string VertexShaderErrorMessage;
		VertexShaderErrorMessage.resize(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cerr << VertexShaderErrorMessage << "\n";
	}

	//Compile Fragment Shader
	std::cerr << "Compiling Shader : " << fragmentShader << " \n";
	char const *fragmentCodeptr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentCodeptr, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		std::string FragmentShaderErrorMessage;
		FragmentShaderErrorMessage.resize(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cerr << FragmentShaderErrorMessage << "\n";
	}

	//Link
	std::cerr << "Linking Program\n";
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
		std::string ProgramErrorMessage;
		ProgramErrorMessage.resize(infoLogLength + 1);
		glGetShaderInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cerr << ProgramErrorMessage << "\n";
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}
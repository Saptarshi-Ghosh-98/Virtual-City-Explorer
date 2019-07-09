#pragma once

#include <GL/glew.h>
#include <tuple>

void create_FBO();
GLuint render_ShadowFBO(std::tuple<int,int,int,float> Terrain_Info, GLuint programID);
void deleteFBO();
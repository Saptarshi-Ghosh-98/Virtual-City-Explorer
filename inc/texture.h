#pragma once

#include <SOIL/SOIL.h>

// Load Image File
unsigned char * loadHeightMap(const char * imagepath, int &width, int &height);

// Load Image File
GLuint loadImage(const char * imagepath);
GLuint loadImage_A(const char * imagepath);

// Load BMP File
GLuint loadBMP(const char * imagepath);

// Load DDS File (Must have Mipmaps)
GLuint loadDDS(const char * imagepath);

// Load CubeMap
GLuint loadCubeMap(const char *imagepath);

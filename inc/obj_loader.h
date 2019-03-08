#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

// Load Wavefront Object Files (*.OBJ)
int loadObject(const char *filename,
				std::vector<glm::vec3> &out_vertex,
				std::vector<glm::vec2> &out_uv,
				std::vector<glm::vec3> &out_normal);

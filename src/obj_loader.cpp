#include "obj_loader.h"

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
				std::vector<glm::vec3> &out_normal) 
{	
	std::cerr << "Loading Object File : ";

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normal;

	std::ifstream file;
	file.open(filename, std::ios::in);

	if (file.peek() == std::istream::traits_type::eof()) {
		std::cerr << filename << " Empty \n";
		return 0;
	}

	std::string line, format;
	std::stringstream ss;
	glm::vec3 vertex3, normal3;
	glm::vec2 uv2;
	int faceVertex[3], faceTex[3], faceNormal[3];
	std::memset(faceVertex, 0, sizeof(faceVertex));
	std::memset(faceTex, 	0, sizeof(faceTex));
	std::memset(faceNormal, 0, sizeof(faceNormal));

	int i = 0, j = 0, k = 0;

	while (file.good()) {
		file >> format;

		if (format == "#") { 				// Comment

			std::getline(file, line);

		} else if (format == "v") {			// Vertex

			file >> vertex3.x >> vertex3.y >> vertex3.z;
			vertex.push_back(vertex3);
		
		} else if (format == "vt") {		// Vertex Texture

			file >> uv2.x >> uv2.y;
			uv.push_back(uv2);

		} else if (format == "vn") {		// Normal

			file >> normal3.x >> normal3.y >> normal3.z;
			normal.push_back(glm::normalize(normal3));

		} else if (format == "f") {			// Face

			std::getline(file, line);
			ss << line;

			/** Valid Formats : Faces must be Triangulated as of now
			 *
			 * f <vertex> <vertex> ...
			 * f <vertex> / <vertexTex>
			 * f <vertex> / <vertexTex> / <vertexNormal>
			 * f <vertex> // <vertexNormal>
			 */
			
			i = j = k = 0;

			ss >> faceVertex[i++];
			
			if (ss.peek() == ' ') {
				while (!ss.eof()) {
					ss >> faceVertex[i++];
					if (ss.eof())
						break;
				}
			} else {
				ss.ignore(1);

				if (ss.peek() == '/') {
					ss.ignore(1);
					ss >> faceNormal[k++];		ss.ignore(1);
					while (!ss.eof()) {
						ss >> faceVertex[i++];	ss.ignore(2);
						ss >> faceNormal[k++];	ss.ignore(1);
						if (ss.eof())	
							break;
					}
				} else {
					ss >> faceTex[j++];
					if (ss.peek() == '/') {
						ss.ignore(1);
						ss >> faceNormal[k++];		ss.ignore(1);
						while (!ss.eof()) {
							ss >> faceVertex[i++];	ss.ignore(1);
							ss >> faceTex[j++];		ss.ignore(1);
							ss >> faceNormal[k++];
							if (ss.eof())
								break;
						}
					} else {
						while (!ss.eof()) {
							ss >> faceVertex[i++];	ss.ignore(1);
							ss >> faceTex[j++];		ss.ignore(1);
							if (ss.eof())
								break;
						}
					}
				}
			}

			for (int i = 0; i < 3; ++i) {
				out_vertex.push_back(vertex[faceVertex[i]-1]);
			}
			if (j == 3) {
				for (int i = 0; i < 3; ++i) {
					out_uv.push_back(uv[faceTex[i]-1]);
				}
			}
			if (k == 3) {
				for (int i = 0; i < 3; ++i) {
					out_normal.push_back(normal[faceNormal[i]-1]);
				}
			}

			ss.clear();
		}

		if (file.eof())
			break;
	}

	std::cerr << filename << " Loaded...\n";
	file.close();
	return true;
}
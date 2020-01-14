#ifndef LOADSHADERS_H
#define LOADSHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

const int nbMatch = 38;
const int nbTeam = 20;
const int nbFaces = 10;
const int res = 10;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
#ifndef CURVE_H
#define CURVE_H

#include <cstdlib>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using namespace glm;

void addCylinder(int cylIdx, int nbFaces, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], int height);
void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], int ranks[], int score[], int nbFaces, int height);

#endif
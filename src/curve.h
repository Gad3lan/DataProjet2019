#ifndef CURVE_H
#define CURVE_H

#include "loadShaders.h"
#include <algorithm>

using namespace glm;

void addCylinder(int cylIdx, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[]);
void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], int ranks[], int score[]);

#endif
#include "curve.h"

void addCylinder(int cylIdx, int nbFaces, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], int height) {
	float r = (height/2)/20.0;
	for (int i = 0; i < nbFaces; i++) {
		float angle = (M_PI/nbFaces)*i;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i] = p1.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+1] = cos(angle)*r+p1.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+2] = sin(angle)*r*p1.z;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+3] = p2.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+4] = cos(angle)*r+p2.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+5] = sin(angle)*r+p2.z;
	}
	for (int i = cylIdx*nbFaces*6; i < (cylIdx+1)*nbFaces*6; i+=3) {
		g_vertex_color_data[i] = color.x;
		g_vertex_color_data[i+1] = color.y;
		g_vertex_color_data[i+2] = color.z;
	}
}

void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], int ranks[], int scores[], int nbFaces, int height) {
	float r = rand()/(float)RAND_MAX;
	float g = rand()/(float)RAND_MAX;
	float b = rand()/(float)RAND_MAX;
	vec4 color = vec4(r, g, b, 1.0);
	for (int i = 0; i < 75; i+=2) {
		float h1 = ((19-ranks[i])/19.0 + scores[i]/98.0);
		float h2 = ((19-ranks[i+1])/19.0 + scores[i+1]/98.0);
		vec3 p1 = vec3((i-19)/19.0, 0, h1);
		vec3 p2 = vec3((i-18.3)/19.0, 0, h1);
		vec3 p3 = vec3((i-18)/19.0, 0, h2);
		addCylinder(i, nbFaces, p1, p2, color, g_vertex_buffer_data, g_vertex_color_data, height);
		addCylinder(i+1, nbFaces, p2, p3, color, g_vertex_buffer_data, g_vertex_color_data, height);
	}
}
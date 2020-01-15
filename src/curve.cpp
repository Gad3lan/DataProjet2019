#include "curve.h"

float dot(vec3 v1, vec3 v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

float max(float a, float b) {
	if (a > b)
		return a;
	return b;
}

void addCylinder(int cylIdx, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[]) {
	float r = .025;
	vec3 normals[nbFaces];
	vec3 light = vec3(0.0,cos(3*M_PI/4),sin(3*M_PI/4)); //Pour les demi cylindres
	//vec3 light = vec3(0.0,cos(M_PI/4),sin(M_PI/4)); //Pour les cylindres entiers
	for (int i = 0; i < nbFaces; i++) {
		float angle;
		if (cylIdx%2 == 0) {
			angle = (M_PI/(nbFaces-1))*i + M_PI/2;
			//angle = (2*M_PI/(nbFaces-1))*i;
		} else {
			angle = (M_PI/(nbFaces-1))*(nbFaces - i - 1) + M_PI/2;
			//angle = (2*M_PI/(nbFaces-1))*(nbFaces-i);
		}
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i] = p1.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+1] = cos(angle)*r + p1.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+2] = sin(angle)*r + p1.z;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+3] = p2.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+4] = cos(angle)*r + p2.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+5] = sin(angle)*r + p2.z;
		normals[i] = vec3(0, cos(angle), sin(angle));
	}

	for (int i = cylIdx*nbFaces*6; i < (cylIdx+1)*nbFaces*6; i+=3) {
		int idx = (i - cylIdx*nbFaces*6)/6;
		float shadow = max(dot(normals[idx], light), 0.2);
		g_vertex_color_data[i] = color.x * shadow;
		g_vertex_color_data[i+1] = color.y * shadow;
		g_vertex_color_data[i+2] = color.z * shadow;
	}
}

float lerp(float point1, float point2, float coef) {
	return (1-coef)*point1 + coef*point2;
}

void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], GLfloat g_vertex_depth_data[], int ranks[], int scores[]) {
	float r = rand()/(float)RAND_MAX;
	float g = rand()/(float)RAND_MAX;
	float b = rand()/(float)RAND_MAX;
	float xOff = nbMatch/2.0;
	vec4 color = vec4(r, g, b, 1.0);
	for (int i = 0; i < nbMatch; i++) {
		float h[res+1];
		h[0] = ((19-ranks[i])/19.0 + scores[i]/98.0)-.475;
		if (i < nbMatch-1) {
			h[res] = ((19-ranks[i+1])/19.0 + scores[i+1]/98.0)-.475;
		} else {
			h[res] = ((19-ranks[i])/19.0 + scores[i]/98.0)-.475;
		}
		for (int j = 1; j < res; j++) {
			float a = M_PI*((float)(j-1)/(res-1))-M_PI/2;
			h[j] = lerp(h[0], h[res], (sin(a)+1)/2);
		}
		float inc = 0;
		for (int j = 0; j < res; j++) {
			vec3 p1 = vec3((float)(i+inc)/xOff - 1, lerp(h[j], h[j+1], float(j)/res)/120., h[j]);
			if (j == 0) {
				inc += .5;
			} else {
				inc += 0.5/(res-1);
			}
			vec3 p2 = vec3((float)(i+inc)/xOff - 1, lerp(h[j], h[j+1], float(j)/res)/120., h[j+1]);
			addCylinder(i*res+j, p1, p2, color, g_vertex_buffer_data, g_vertex_color_data);
			for (int k = 0; k < nbFaces*6; k++) {
				g_vertex_depth_data[((i*res+j)*nbFaces*6) + k] = lerp(h[j], h[j+1], float(j)/res)/120.;
			}
		}
	}
}
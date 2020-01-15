#include "curve.h"


float dot(vec3 v1, vec3 v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

float max(float a, float b) {
	return (a > b) ? a : b;
}

float lerp(float point1, float point2, float coef) {
	return (1-coef)*point1 + coef*point2;
}

void addCylinder(int cylIdx, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[]) {
	// Initialisation des parametres du cylindre
	float r = .025;
	vec3 normals[nbFaces];
	vec3 light = vec3(0.0,cos(M_PI/6),sin(M_PI/6));

	// Création du cylindre
	for (int i = 0; i < nbFaces; i++) {
		float angle;
		if (cylIdx%2 == 0) {
			angle = (M_PI/(nbFaces-1))*i - M_PI/2;
		} else {
			angle = (M_PI/(nbFaces-1))*(nbFaces - i - 1) - M_PI/2;
		}
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i] = p1.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+1] = cos(angle)*r + p1.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+2] = sin(angle)*r + p1.z;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+3] = p2.x;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+4] = cos(angle)*r + p2.y;
		g_vertex_buffer_data[(cylIdx*nbFaces*6)+6*i+5] = sin(angle)*r + p2.z;
		normals[i] = vec3(0, cos(angle), sin(angle));
	}

	// Génération de la couleur du cylindre et de son ombre
	for (int i = cylIdx*nbFaces*6; i < (cylIdx+1)*nbFaces*6; i+=3) {
		int idx = (i - cylIdx*nbFaces*6)/6;
		float shadow = max(dot(normals[idx], light), 0.2);
		g_vertex_color_data[i] = color.x * shadow;
		g_vertex_color_data[i+1] = color.y * shadow;
		g_vertex_color_data[i+2] = color.z * shadow;
	}
}

void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], GLfloat g_vertex_depth_data[], int ranks[], int scores[], vec4 color) {
	float xOff = nbMatch/2.0; // Décalage à gauche pour centrer le gapchart

	// Génération de la ligne
	for (int i = 0; i < nbMatch; i++) {
		// Calcul de la hauteur de chaque point
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

		// Génération des cylindres
		float inc = 0;
		for (int j = 0; j < res; j++) {
			// Calcul des points de départ et d'arrivé du cylindre
			vec3 p1 = vec3((float)(i+inc)/xOff - 1, lerp(h[j], h[j+1], float(j)/res)/120., h[j]);
			if (j == 0) {
				inc += .5;
			} else {
				inc += 0.5/(res-1);
			}
			vec3 p2 = vec3((float)(i+inc)/xOff - 1, lerp(h[j], h[j+1], float(j)/res)/120., h[j+1]);

			// Création du cylindre
			addCylinder(i*res+j, p1, p2, color, g_vertex_buffer_data, g_vertex_color_data);

			// Calcul de la profondeur des points du cylindre
			for (int k = 0; k < nbFaces*6; k++) {
				g_vertex_depth_data[((i*res+j)*nbFaces*6) + k] = lerp(ranks[i], ranks[max(i+1, nbMatch-1)], float(j)/res)/60.;
			}
		}
	}
}
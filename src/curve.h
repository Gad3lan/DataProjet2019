#ifndef CURVE_H
#define CURVE_H

#include "loadShaders.h"
#include <algorithm>

using namespace glm;

/**
 * Crée un cylindre entre les points p1 et p2.
 * 
 * @param cylIdx L'indexe du cylindre pour le placer au bon endroit de le buffer
 * @param p1 Point de départ
 * @param p2 Point d'arrivée
 * @param color Couleur du cylindre
 * @param g_vertex_buffer_data Contient les positions du cylindre
 * @param g_vertex_color_data Contient les couleurs du cylindre
 */
void addCylinder(int cylIdx, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[]);
/**
 * Génère une ligne complète et rempli les vertex data
 * 
 * @param g_vertex_buffer_data Contient les positions des cylindres
 * @param g_vertex_color_data Contient les couleurs des cylindre
 * @param g_vertex_depth_data Contient le z-buffer des cylindres
 * @param ranks Rangs de l'equipe en fonction des matchs
 * @param scores Scores de l'equipe en fonction des matchs
 * @param color Couleur de la ligne
 */
void generateCurve(GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[], GLfloat g_vertex_depth_data[], int ranks[], int score[], vec4 color);

#endif
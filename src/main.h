#ifndef MAIN_H
#define MAIN_H

#include "loadShaders.h"
#include "csvReader.h"
#include "curve.h"

using namespace glm;
using namespace std;

/**
 * Change les valeurs des angles si l'utilisateur appuie sur ZQSD ou les fleches directionnelles.
 * 
 * @param yaw Rotation sur l'axe Y
 * @param roll Rotation sur l'axe X
 */
void rotate(float &yaw, float &roll);

/**
 * Change la valeur du zoom si l'utilisateur appuis sur espace ou left maj.
 * 
 * @param zoom 
 */
void scale(float &zoom);

/**
 * Met a jour le tableau des touches si il y a un changement d'etat sur l'une de celles-ci.
 */
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

#endif
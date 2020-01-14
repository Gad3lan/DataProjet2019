// sous mac 
// g++ -I/usr/local/include/ -lglfw -lGLEW main.cpp -framework OpenGL  -omain 
// ./main

// sous linux 	
// g++ -I/usr/local/include/ -I/public/ig/glm/ -c main.cpp  -omain.o
// g++ -I/usr/local main.o -lglfw  -lGLEW  -lGL  -omain
// ./main

#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

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

#include "loadShaders.h"
#include "csvReader.h"
#include "curve.h"

#include <unistd.h>

using namespace glm;
using namespace std;

// stocke les variables uniformes qui seront communes a tous les vertex dessines
GLint uniform_proj, uniform_view, uniform_model;
bool keys[1024];

void rotate(float &yaw, float &roll) {
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		yaw += M_PI/400;
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		yaw -= M_PI/400;
	}
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		if (roll < M_PI/2)
			roll += M_PI/400;
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		if (roll > -M_PI/2)
		roll -= M_PI/400;
	}
	if (keys[GLFW_KEY_R]) {
		yaw = 3*M_PI/2;
		roll = M_PI/6;
	}
}

float scale(float &zoom) {
	if (keys[GLFW_KEY_SPACE]) {
		zoom -= 0.01;
	}
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		zoom += 0.01;
	}
	if (keys[GLFW_KEY_R]) {
		zoom = 1.0;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode ) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if(action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if(action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

int main() {
	string rawCSV = readCSVFile("data/rankspts.csv");
	string teams[nbTeam];
	int ranks[nbTeam][nbMatch];
	int scores[nbTeam][nbMatch];

	parse(rawCSV, teams, scores, ranks);
	//aff(scores);
	rawCSV = "";

	if( !glfwInit() ) {

	    fprintf( stderr, "Failed to initialize GLFW\n" );
	  
	    return -1;
	} else {

	}

	glfwWindowHint(GLFW_SAMPLES, 2); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // On ne veut pas l'ancien OpenGL
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  
	// Ouvre une fenêtre et crée son contexte OpenGl
 // (Dans le code source qui accompagne, cette variable est globale)
	GLFWwindow* window = glfwCreateWindow( 1024, 768, "Projet", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	
	    return -1;
	}

	int nbVertex = 6*nbFaces*nbMatch*res;
	GLfloat g_vertex_buffer_data[nbTeam][nbVertex];
	GLfloat g_vertex_color_data[nbTeam][nbVertex];
	for (int i=0; i<nbTeam; i++){
		generateCurve(g_vertex_buffer_data[i], g_vertex_color_data[i], ranks[i], scores[i]);
	}
	
	glfwMakeContextCurrent(window); // Initialise GLEW
	glewExperimental=true; // Nécessaire dans le profil de base
	if (glewInit() != GLEW_OK) {
	    fprintf(stderr, "Failed to initialize GLEW\n");
	  
	    return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glDepthRange(-1, 1);
	// Bon maintenant on cree le VAO et cette fois on va s'en servir !
  GLuint VertexArrayID[nbTeam];
	GLuint vertexbuffer[nbTeam];
	glGenVertexArrays(nbTeam, VertexArrayID);
	glGenBuffers(nbTeam, vertexbuffer);
	for (int i = 0; i < nbTeam; i++) {
		glBindVertexArray(VertexArrayID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data[i])+sizeof(g_vertex_color_data[i]), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data[i]), g_vertex_buffer_data[i]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data[i]), sizeof(g_vertex_color_data[i]), g_vertex_color_data[i]);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer( // same thing for the colors
			1, 
			3, 
			GL_FLOAT, 
			GL_FALSE, 
			0, 
			(void*)sizeof(g_vertex_buffer_data[i])
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray (0);

	// Assure que l'on peut capturer la touche d'échappement enfoncée ci-dessous
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback( window, KeyCallback );

	GLuint programID = LoadShaders( "src/SimpleVertexShader5.vs", "src/SimpleFragmentShader5.frag" );
  uniform_proj     = glGetUniformLocation(programID, "projectionMatrix");
	uniform_view     = glGetUniformLocation(programID, "viewMatrix");
	uniform_model    = glGetUniformLocation(programID, "modelMatrix");

 
	double lastTime = glfwGetTime();
 	int nbFrames = 0;
	float yaw = 3*M_PI/2;
	float roll = M_PI/6;
	float zoom = 1.0;

	do{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
				// printf and reset timer
				printf("FPS: %d\n", nbFrames);
				nbFrames = 0;
				lastTime += 1.0;
		}
		// clear before every draw 1

		rotate(yaw, roll);
		scale(zoom);
		if (zoom < 0.1)
			zoom = 0.1;
    // Use our shader program
		glUseProgram(programID);

		// onchange de matrice de projection : la projection orthogonale est plus propice a la visualization !
		//glm::mat4 projectionMatrix = glm::perspective(glm::radians(zoom), 1024.0f / 768.0f, 0.1f, 1000.0f);
		glm::mat4 projectionMatrix = glm::ortho( -zoom, zoom, -zoom, zoom, -3.f, 3.f );
		glm::mat4 viewMatrix       = glm::lookAt(
						                      vec3(cos(yaw), sin(yaw), sin(roll)), // where is the camara
						                      vec3(0,0,0.5), //where it looks
						                      vec3(0,0,1) // head is up
						                    );
		glm::mat4 modelMatrix      = glm::mat4(1.0);

    glUniformMatrix4fv(uniform_proj,  1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniform_view,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		for (int i = 0; i < nbTeam; i++) {
			// on re-active le VAO avant d'envoyer les buffers
			glBindVertexArray(VertexArrayID[i]);

			// Draw the triangle(s) !
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(g_vertex_buffer_data[i])/(3*sizeof(float))); // Starting from vertex 0 .. all the buffer 

			// on desactive le VAO a la fin du dessin
		}
		glBindVertexArray (0);
		// on desactive les shaders
		glUseProgram(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    // apres avoir recupere les evenements, on teste si la touche E est pressee et si c'est le cas
    // on re-genere des donnees
	} // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );

}

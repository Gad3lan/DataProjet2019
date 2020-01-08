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

#include "loadShaders.h"
#include "csvReader.h"
#include "curve.h"

#include <unistd.h>

using namespace glm;
using namespace std;

const int N = 40;
const int nbMatch = 38;
const int nbTeam = 20;
// stocke les variables uniformes qui seront communes a tous les vertex dessines
GLint uniform_proj, uniform_view, uniform_model;

int main() {
	string rawCSV = readCSVFile("data/rankspts.csv");
	string teams[nbTeam];
	int ranks[nbTeam][nbMatch];
	int scores[nbTeam][nbMatch];

	parse(rawCSV, teams, scores, ranks);
	aff(scores);

	if( !glfwInit() ) {
	    fprintf( stderr, "Failed to initialize GLFW\n" );
	  
	    return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // On ne veut pas l'ancien OpenGL
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  
	// Ouvre une fenêtre et crée son contexte OpenGl
	GLFWwindow* window; // (Dans le code source qui accompagne, cette variable est globale)
	int width = 1024;
	int height = 698;
	window = glfwCreateWindow( width, height, "Main 05", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	  
	    return -1;
	}

	int nbFaces = 3;
	int nbVertex = 6*nbFaces*nbMatch*2;
	GLfloat g_vertex_buffer_data[nbTeam][nbVertex];
	GLfloat g_vertex_color_data[nbTeam][nbVertex];
	for (int i=0; i<nbTeam; i++){
		generateCurve(g_vertex_buffer_data[i], g_vertex_color_data[i], ranks[i], scores[i], nbFaces, height);
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
			(void*)sizeof(g_vertex_buffer_data));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray (0);
	}

	// Assure que l'on peut capturer la touche d'échappement enfoncée ci-dessous
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint programID = LoadShaders( "src/SimpleVertexShader5.vertexshader", "src/SimpleFragmentShader5.fragmentshader" );
  uniform_proj     = glGetUniformLocation(programID, "projectionMatrix");
	uniform_view     = glGetUniformLocation(programID, "viewMatrix");
	uniform_model    = glGetUniformLocation(programID, "modelMatrix");

 
  float angle = 0.0f;

	do{
		cout << 1 << endl;
		angle += M_PI/200;
		// clear before every draw 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader program
		glUseProgram(programID); 
		// onchange de matrice de projection : la projection orthogonale est plus propice a la visualization !
		//glm::mat4 projectionMatrix = glm::perspective(glm::radians(66.0f), 1024.0f / 768.0f, 0.1f, 200.0f);
		glm::mat4 projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -3.f, 3.f );
		glm::mat4 viewMatrix       = glm::lookAt(
						                      vec3(1.5*cos(angle), 1.5*sin(angle), -0.5), // where is the camara
						                      vec3(0,0,0.5), //where it looks
						                      vec3(0,0, 1) // head is up
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
			glBindVertexArray (0);
		}
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

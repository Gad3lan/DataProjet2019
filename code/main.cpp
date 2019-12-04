// sous mac 
// g++ -I/usr/local/include/ -lglfw -lGLEW main.cpp -framework OpenGL  -omain 
// ./main

// sous linux 	
// g++ -I/usr/local/include/ -I/public/ig/glm/ -c main.cpp  -omain.o
// g++ -I/usr/local main.o -lglfw  -lGLEW  -lGL  -omain
// ./main

#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


const int N = 40;
// stocke les variables uniformes qui seront communes a tous les vertex dessines
GLint uniform_proj, uniform_view, uniform_model;


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

using namespace std;

string readCSVFile(string name) {
	ifstream file;
	string content;
	string line;
	file.open(name);
	while (getline(file, line)) {
		content += line + "\n";
	}
	file.close();
	return content;
}

void parse(string csv, string teams[20], int scores[20][38], int ranks[20][38]) {
	istringstream iss(csv); 
	for (int i = 0; i < 20; i++) {
		string line;
		getline(iss, line, '\n');
		istringstream streamLine(line);
		getline(streamLine, teams[i], ',');
		for (int j = 0; j < 38; j++) {
			string tmp;
			getline(streamLine, tmp, ',');
			ranks[i][j] = stoi(tmp);
			getline(streamLine, tmp, ',');
			scores[i][j] = stoi(tmp);
			for (int k = 0; k < 4; k++) {
				getline(streamLine, tmp, ',');
			}
		}
	}
}

void aff(int scores[20][38]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			cout << scores[i][j] << " ";
		}
		cout << endl;
	}
}



void addCylinder(int cylIdx, int faceNb, vec3 p1, vec3 p2, vec4 color, GLfloat g_vertex_buffer_data[], GLfloat g_vertex_color_data[]) {
	float r = 0.0002;
	for (int i = 0; i < faceNb; i++) {
		float angle = (M_PI/faceNb)*i;
		float nextAngle = (M_PI/faceNb)*(i+1);
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i] = p2.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+1] = cos(angle)*r+p2.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+2] = sin(angle)*r*p2.z;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+3] = p2.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+4] = cos(nextAngle)*r+p2.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+5] = sin(nextAngle)*r+p2.z;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+6] = p1.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+7] = cos(angle)*r+p1.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+8] = sin(angle)*r+p1.z;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+9] = p2.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+10] = cos(nextAngle)*r+p2.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+11] = sin(nextAngle)*r+p2.z;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+12] = p1.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+13] = cos(nextAngle)*r+p1.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+14] = sin(nextAngle)*r+p1.z;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+15] = p1.x;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+16] = cos(angle)*r+p1.y;
		g_vertex_buffer_data[(cylIdx*faceNb*18)+18*i+17] = sin(angle)*r+p1.z;
	}
}


int main() {
	string rawCSV = readCSVFile("../data/rankspts.csv");
	string teams[20];
	int ranks[20][38];
	int scores[20][38];

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
	window = glfwCreateWindow( 1024, 768, "Main 05", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	  
	    return -1;
	}

		int height;
	int width;
	glfwGetWindowSize(window,&width, &height);
	float coefHeight = 0.5/21;
	float coefWidth =1.0/20;
	int nbFaces = 10;
	int nbVertex = 18*nbFaces*38*20;
	GLfloat g_vertex_buffer_data[nbVertex];
	GLfloat g_vertex_color_data[nbVertex];
	for (int j=0; j<20; j++){
		for(int i=0;i<37;i++){
			addCylinder(j*20+i, nbFaces, vec3((i-19)/19.0, 0, ranks[j][i]*coefWidth),vec3((i-18)/19.0, 0, ranks[j][i+1]*coefWidth), vec4(1,1,1,1), g_vertex_buffer_data, g_vertex_color_data); 
		}
	}
	for (int i = 0; i < nbVertex; i++) {
		g_vertex_color_data[i] = 1.0;
	
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
  GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	  // Only allocqte memory. Do not send yet our vertices to OpenGL.
	  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data)+sizeof(g_vertex_color_data), 0, GL_STATIC_DRAW);
      // send vertices in the first part of the buffer
	  glBufferSubData(GL_ARRAY_BUFFER, 0,                            sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
	  // send colors in the second part of the buffer
	  glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), sizeof(g_vertex_color_data), g_vertex_color_data);
	
		// ici les commandes stockees "une fois pour toute" dans le VAO 
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

	// on desactive le VAO a la fin de l'initialisation
	glBindVertexArray (0);

	// Assure que l'on peut capturer la touche d'échappement enfoncée ci-dessous
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint programID = LoadShaders( "SimpleVertexShader5.vertexshader", "SimpleFragmentShader5.fragmentshader" );
  uniform_proj     = glGetUniformLocation(programID, "projectionMatrix");
	uniform_view     = glGetUniformLocation(programID, "viewMatrix");
	uniform_model    = glGetUniformLocation(programID, "modelMatrix");

 
  float angle = 0.0f;

	do{
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
		mat4 modelMatrix      = glm::mat4(1.0);

    glUniformMatrix4fv(uniform_proj,  1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniform_view,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    // on re-active le VAO avant d'envoyer les buffers
    glBindVertexArray(VertexArrayID);

    // Je met en commentaire tous les trucs qui n'ont plus besoin d'etre appelle a chaque dessin !
	  // 1rst attribute buffer : vertices
		/*glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
		glEnableVertexAttribArray(1);*/

		// Draw the triangle(s) !
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data)/(3*sizeof(float))); // Starting from vertex 0 .. all the buffer 
		// Ca aussi on peut l'enlever puisque le enable n'est plus fait ici !
		/*glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);*/

		// on desactive le VAO a la fin du dessin
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

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



/**GLfloat* loadCoord(int ranks[][]){
	float coefHeight = height/21;
	float coefWidth = weight/39;
	GLfloat g_vertex_buffer_data[38];
	for (int j=0; j<20; j++){
		for(int i=0;i<228;i+6){
			g_vertex_buffer_data[i] = (ranks[j][i]+1)*coefWidth;
			g_vertex_buffer_data[i+1] = (ranks[j][i]+1)*coefHeight;
			g_vertex_buffer_data[i+2] = 0;
			g_vertex_buffer_data[i+3] = (ranks[j][i+1]+1)*coefWidth;
			g_vertex_buffer_data[i+4] = (ranks[j][i+1]+1)*coefHeight;
			g_vertex_buffer_data[i+5] = 0;
		}
	}
		return g_vertex_buffer_data[];
}
**/


int main() {
	string rawCSV = readCSVFile("../data/rankspts.csv");
	string teams[20];
	int ranks[20][38];
	int scores[20][38];
	cout<<__LINE__<<endl;
	parse(rawCSV, teams, scores, ranks);
	aff(scores);

	
	if( !glfwInit() ) {
	    fprintf( stderr, "Failed to initialize GLFW\n" );
	    cout<<__LINE__<<endl;
	    return -1;
	}




	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // On ne veut pas l'ancien OpenGL
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  	cout<<__LINE__<<endl;
	// Ouvre une fenêtre et crée son contexte OpenGl
	GLFWwindow* window; // (Dans le code source qui accompagne, cette variable est globale)
	window = glfwCreateWindow( 1024, 768, "Main 05", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	    cout<<__LINE__<<endl;
	    return -1;
	}

		int height;
	int width;
	glfwGetWindowSize(window,&width, &height);
	float coefHeight = 0.5/21;
	float coefWidth =1.0/20;
	GLfloat g_vertex_buffer_data[4560];
	for (int j=0; j<20; j++){
		for(int i=0;i<37;i++){
			g_vertex_buffer_data[j*228+6*i+0] = (i-19)/19.0;
			g_vertex_buffer_data[j*228+6*i+1] = 0;
			g_vertex_buffer_data[j*228+6*i+2] = (ranks[j][i])*coefWidth;
			g_vertex_buffer_data[j*228+6*i+3] = (i-18)/19.0;
			g_vertex_buffer_data[j*228+6*i+4] = 0;
			g_vertex_buffer_data[j*228+6*i+5] =(ranks[j][i+1])*coefWidth;
			cout<<__LINE__<<endl;
		}
	}
	GLfloat g_vertex_color_data[4560];
	for (int i = 0; i < 4560; i++) {
		g_vertex_color_data[i] = 1.0;
		cout<<__LINE__<<endl;
	}

	
	
	
	
	
	
	
	
	glfwMakeContextCurrent(window); // Initialise GLEW
	glewExperimental=true; // Nécessaire dans le profil de base
	if (glewInit() != GLEW_OK) {
	    fprintf(stderr, "Failed to initialize GLEW\n");
	    cout<<__LINE__<<endl;
	    return -1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glDepthRange(-1, 1);
cout<<__LINE__<<endl;
	// Bon maintenant on cree le VAO et cette fois on va s'en servir !
  GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
cout<<__LINE__<<endl;
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
cout<<__LINE__<<endl;
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	  // Only allocqte memory. Do not send yet our vertices to OpenGL.
	  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data)+sizeof(g_vertex_color_data), 0, GL_STATIC_DRAW);
cout<<__LINE__<<endl;
      // send vertices in the first part of the buffer
	  glBufferSubData(GL_ARRAY_BUFFER, 0,                            sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
cout<<__LINE__<<endl;
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
cout<<__LINE__<<endl;
    glVertexAttribPointer( // same thing for the colors
    	1, 
    	3, 
    	GL_FLOAT, 
    	GL_FALSE, 
    	0, 
    	(void*)sizeof(g_vertex_buffer_data));
		glEnableVertexAttribArray(1);
cout<<__LINE__<<endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// on desactive le VAO a la fin de l'initialisation
	glBindVertexArray (0);
cout<<__LINE__<<endl;

	// Assure que l'on peut capturer la touche d'échappement enfoncée ci-dessous
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint programID = LoadShaders( "SimpleVertexShader5.vertexshader", "SimpleFragmentShader5.fragmentshader" );
  uniform_proj     = glGetUniformLocation(programID, "projectionMatrix");
	uniform_view     = glGetUniformLocation(programID, "viewMatrix");
	uniform_model    = glGetUniformLocation(programID, "modelMatrix");

 cout<<__LINE__<<endl; 
  float angle = 0.0f;

	do{
		angle += M_PI/200;
		// clear before every draw 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader program
		glUseProgram(programID); 
cout<<__LINE__<<endl;
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
cout<<__LINE__<<endl;
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
		glDrawArrays(GL_LINES, 0, sizeof(g_vertex_buffer_data)/(3*sizeof(float))); // Starting from vertex 0 .. all the buffer 
cout<<__LINE__<<endl;
		// Ca aussi on peut l'enlever puisque le enable n'est plus fait ici !
		/*glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);*/

		// on desactive le VAO a la fin du dessin
		glBindVertexArray (0);
cout<<__LINE__<<endl;
		// on desactive les shaders
		glUseProgram(0);

cout<<__LINE__<<endl;
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
cout<<__LINE__<<endl;
    // apres avoir recupere les evenements, on teste si la touche E est pressee et si c'est le cas
    // on re-genere des donnees
	} // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );
	cout<<__LINE__<<endl;
}

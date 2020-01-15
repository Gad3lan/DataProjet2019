#include "main.h"

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

void scale(float &zoom) {
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

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
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
	// Récupération des données
	string rawCSV = readCSVFile("data/rankspts.csv");
	string teams[nbTeam];
	int ranks[nbTeam][nbMatch];
	int scores[nbTeam][nbMatch];
	parse(rawCSV, teams, scores, ranks);

	// Initialisation de GLWF
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	// Paramétrage de GLFW
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  
	// Création de la fenêtre
	GLFWwindow* window = glfwCreateWindow( 1024, 768, "Projet", NULL, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	
	    return -1;
	}

	// Création et remplissages des lignes
	int nbVertex = 6*nbFaces*nbMatch*res;
	GLfloat g_vertex_buffer_data[nbTeam][nbVertex];
	GLfloat g_vertex_color_data[nbTeam][nbVertex];
	GLfloat g_vertex_depth_data[nbTeam][nbVertex];
	for (int i=0; i<nbTeam; i++){
		// Couleurs aléatoires
		/*float r = rand()/(float)RAND_MAX;
		float g = rand()/(float)RAND_MAX;
		float b = rand()/(float)RAND_MAX;*/

		//Couleurs du sujet
		vec4 color;
		if (i < 4) {
			color = vec4(0.6, 1.0, 1.0, 1.0);
		} else if (i >= 4 && i < 7) {
			color = vec4(1.0, 1.0, 0.6, 1.0);
		} else if (i >= 16) {
			color = vec4(1.0, 0.4, 0.4, 1.0);
		} else {
			color = vec4(0.8, 0.8, 0.8, 1.0);
		}
		generateCurve(g_vertex_buffer_data[i], g_vertex_color_data[i], g_vertex_depth_data[i], ranks[i], scores[i], color);
	}
	
	// Initialisation de GLEW
	glfwMakeContextCurrent(window);
	glewExperimental=true;
	if (glewInit() != GLEW_OK) {
	    fprintf(stderr, "Failed to initialize GLEW\n");
	    return -1;
	}

	// Parametrage de la profondeur
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthRange(-1, 1);

	// Création des VAOs
  GLuint VertexArrayID[nbTeam];
	GLuint vertexbuffer[nbTeam];
	glGenVertexArrays(nbTeam, VertexArrayID);
	glGenBuffers(nbTeam, vertexbuffer);
	for (int i = 0; i < nbTeam; i++) {
		glBindVertexArray(VertexArrayID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data[i])+sizeof(g_vertex_color_data[i])+sizeof(g_vertex_depth_data[i]), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data[i]), g_vertex_buffer_data[i]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data[i]), sizeof(g_vertex_color_data[i]), g_vertex_color_data[i]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data[i])+sizeof(g_vertex_color_data[i]), sizeof(g_vertex_depth_data[i]), g_vertex_depth_data[i]);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			1, 
			3, 
			GL_FLOAT, 
			GL_FALSE, 
			0, 
			(void*)sizeof(g_vertex_buffer_data[i])
		);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			2, 
			3, 
			GL_FLOAT, 
			GL_FALSE, 
			0, 
			(void*)sizeof(g_vertex_depth_data[i])
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray (0);

	// Initialisation de la gestion du clavier
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback( window, KeyCallback );

	// Initialisation des shaders
	GLuint programID = LoadShaders( "src/SimpleVertexShader5.vs", "src/SimpleFragmentShader5.frag" );
  uniform_proj     = glGetUniformLocation(programID, "projectionMatrix");
	uniform_view     = glGetUniformLocation(programID, "viewMatrix");
	uniform_model    = glGetUniformLocation(programID, "modelMatrix");

	// Création des variables de la camera et boucle d'affichage
	double lastTime = glfwGetTime();
 	int nbFrames = 0;
	float yaw = 3*M_PI/2;
	float roll = M_PI/6;
	float zoom = 1.0;
	do{
		// Nettoyage de la précédente image et fond blanc
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compteur de framerate
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){
				printf("FPS: %d\n", nbFrames);
				nbFrames = 0;
				lastTime += 1.0;
		}

		// Mise a jour des infos de la caméra
		rotate(yaw, roll);
		scale(zoom);
		if (zoom < 0.1)
			zoom = 0.1;

		// Chargement des shaders
		glUseProgram(programID);

		// Parametrage de la caméra
		glm::mat4 projectionMatrix = glm::ortho( -zoom, zoom, -zoom, zoom, -3.f, 3.f );
		glm::mat4 viewMatrix       = glm::lookAt(
						                      vec3(cos(yaw), sin(yaw), sin(roll)),
						                      vec3(0,0,0.5),
						                      vec3(0,0,1)
						                    );
		glm::mat4 modelMatrix      = glm::mat4(1.0);
    glUniformMatrix4fv(uniform_proj,  1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniform_view,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// Dessin des lignes
		for (int i = 0; i < nbTeam; i++) {
			glBindVertexArray(VertexArrayID[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(g_vertex_buffer_data[i])/(3*sizeof(float))); 
		}
		glBindVertexArray (0);
		glUseProgram(0);

		// Fin de l'image
    glfwSwapBuffers(window);
    glfwPollEvents();
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );
}

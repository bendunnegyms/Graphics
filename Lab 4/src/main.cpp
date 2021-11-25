// Windows includes (For Time, IO, etc.)
#include <ctime>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "shader.h"
#include "model.h"
#include "camera.h"


using namespace std;

int width = 1200;
int height = 600;


GLfloat rotate_y = 0.0f;
GLfloat rotate_x = 0.0f;
GLfloat rotate_z = 0.0f;
vec3 vector_1 = vec3(0.0f, 0.0f, 0.0f);
mat4 mview;
float x_1 = 0;
float y_1 = 0;



// -- -- -- PROJECT GLOBALS -- -- --
//
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
Model deer;
Shader shader;
glm::vec3 light_position_world = glm::vec3(0.0, 15.0, 12.0);
float delta_time = 0.0f;
float last_time = 0.0f;
bool first_mouse = true;
float last_X = 0.0f;
float last_Y = 0.0f;
float model_pos_z = 0.0f;


//
// -- -- -- -- -- --- -- -- -- -- --

void init()
{
	// Set up the shaders
	shader = Shader("media/modelVertexShader.txt","media/modelFragmentShader.txt");
	deer = Model("media/deer.fbx",true);
	
}

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); 
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	shader.use();
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
    shader.setMat4("view", view);
	glm::vec3 view_position = camera.GetViewPosition();
    shader.setVec3("view_position", view_position);
    shader.setVec3("light_position", light_position_world);

	//glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0));
	//deer.setTransform("Body", model);

	//BACK LEFT LEG TRANSFORMS
	glm::mat4 UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-6,2.4f));
	deer.addTransform("UBL", UBL_translate);
	glm::mat4 UBL_rotate = glm::rotate(glm::mat4(1.0f), cos(last_time*0.005f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UBL", UBL_rotate);
	UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,6,-2.4f));
	deer.addTransform("UBL", UBL_translate);

	glm::mat4 LBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-3,3.0f));
	deer.addTransform("LBL", LBL_translate);
	glm::mat4 LBL_rotate = glm::rotate(glm::mat4(1.0f), sin(last_time*0.005f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LBL", LBL_rotate);
	LBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,3,-3.0f));
	deer.addTransform("LBL", LBL_translate);
	

	//BACK RIGHT LEG TRANSFORMS
	glm::mat4 UBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-6,2.4f));
	deer.addTransform("UBR", UBR_translate);
	glm::mat4 UBR_rotate = glm::rotate(glm::mat4(1.0f), sin(last_time*0.005f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UBR", UBR_rotate);
	UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,6,-2.4f));
	deer.addTransform("UBR", UBL_translate);
	
	glm::mat4 LBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-3,3.0f));
	deer.addTransform("LBR", LBR_translate);
	glm::mat4 LBR_rotate = glm::rotate(glm::mat4(1.0f), cos(last_time*0.005f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LBR", LBR_rotate);
	LBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,3,-3.0f));
	deer.addTransform("LBR", LBR_translate);

	//glm::mat4 body_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-6, 3));
	//deer.addTransform("Body", body_translate);
	//glm::mat4 body_rotate = glm::rotate(glm::mat4(1.0f), sin(last_time*0.005f) * 1.0f, glm::vec3(1,0,0));
	//deer.addTransform("Body", body_rotate);
	//body_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,6,-3));
	//deer.addTransform("Body", body_translate);
	

	deer.compileTransforms();
	deer.Draw(shader);
	deer.resetTransforms();
	glPopMatrix(); 		// Don't forget to pop the Matrix
	glutSwapBuffers();
}



void updateScene() {

	int time_now = glutGet(GLUT_ELAPSED_TIME);
	delta_time = time_now - last_time;
	model_pos_z += delta_time;
    last_time = time_now;
	glutPostRedisplay();
}




void keypress(unsigned char key, int x, int y) {
	if (key == 'w') {
		camera.ProcessKeyboard(FORWARD, delta_time * 0.01f);
	}

	if (key == 'a')
	{
		camera.ProcessKeyboard(LEFT, delta_time * 0.01f);
	}
	if (key == 's')
	{
		camera.ProcessKeyboard(BACKWARD, delta_time * 0.01f);
	}
	if (key == 'd')
	{
		camera.ProcessKeyboard(RIGHT, delta_time * 0.01f);
	}
	if (key == 'q'){
		camera.ProcessKeyboard(UP, delta_time * 0.01f);
	}
	if (key == 'e'){
		camera.ProcessKeyboard(DOWN, delta_time * 0.01f);
	}
	
	glutPostRedisplay(); // Redraw the scene
}

void mousePressed(int button, int state, int x, int y) {
	//zoom out
	if (button == 3) {
		printf("scroll up");
		vector_1 += vec3(0.0f, 0.0f, -0.05f);
	}
	//zoom in
	else if (button == 4) {
		printf("scroll down");
		vector_1 -= vec3(0.0f, 0.0f, -0.05f);
	}
}

void mouseMoved(int x_2, int y_2) {
	if (first_mouse)
    {
        last_X = x_2;
        last_Y = y_2;
        first_mouse = false;
    }
    float xoffset = x_2 - last_X;
    float yoffset = last_Y - y_2; // reversed since y-coordinates go from bottom to top
    last_X = x_2;
    last_Y = y_2;
    camera.ProcessMouseMovement(xoffset, yoffset);
	//printf("mouse moved, %i, %i\n", xoffset, yoffset);
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutMouseFunc(mousePressed);
	glutPassiveMotionFunc(mouseMoved);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}

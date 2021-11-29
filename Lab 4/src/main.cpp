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

#define PI 3.14159265

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
Model city;
Model skybox;
Shader skybox_shader;
Shader deer_shader;
Shader city_shader;

glm::vec3 light_position_world = glm::vec3(0.0, 15.0, 12.0);
float delta_time = 0.0f;
float last_time = 0.0f;
bool first_mouse = true;
float last_X = 0.0f;
float last_Y = 0.0f;
float model_pos_z = 0.0f;

vector<Model> deer_crowd;
int crowd_size = 16;

//
// -- -- -- -- -- --- -- -- -- -- --

void init()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	//glDepthMask(GL_TRUE);  
	// Set up the shaders
	deer_shader = Shader("media/modelVertexShader.txt","media/modelFragmentShader.txt");
	skybox_shader = Shader("media/simpleVertexShader.txt","media/simpleFragmentShader.txt");
	city_shader = Shader("media/modelVertexShader.txt","media/modelFragmentShader.txt");
	for (int i=0; i<crowd_size; i++){
		//INITIALISE DEER STARTING POS IN A CIRCLE AROUND THE ORIGIN
		float i_float = (float) i;
		float offset = (i>8 ? 2 : 1);
		deer_crowd.push_back(Model("media/deer.fbx",true, (1 * offset * cos(PI * (i_float/4))), (1 * offset * sin(PI * (i_float/4))), last_time));
	}
	skybox = Model("media/skybox.obj",true,0,0, last_time);
	city = Model("media/city/city.obj",true,0,0, last_time, true);
}


void animate_deer(Model deer){
	//ANIMATE DEER
	
	//BODY BOB
	glm::mat4 BODY_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-7.0f, 3.0f));
	deer.addTransform("Body", BODY_translate);
	glm::mat4 BODY_rotate = glm::rotate(glm::mat4(1.0f), cos(deer.movement_parameter*0.006f) * 0.05f, glm::vec3(1,0,0));
	deer.addTransform("Body", BODY_rotate);
	BODY_translate = glm::translate(BODY_translate, glm::vec3(0,7.0f, -3.0f));
	deer.addTransform("Body", BODY_translate);
	glm::mat4 BODY_scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
	deer.addTransform("Body", BODY_scale);
	

	//HEAD BOB
	glm::mat4 HEAD_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-7.0f, -3.0f));
	deer.addTransform("Head", HEAD_translate);
	glm::mat4 HEAD_rotate = glm::rotate(glm::mat4(1.0f), sin(deer.movement_parameter*0.006f) * 0.08f, glm::vec3(1,0,0));
	deer.addTransform("Head", HEAD_rotate);
	HEAD_translate = glm::translate(HEAD_translate, glm::vec3(0,14.0f, 6.0f));
	deer.addTransform("Head", HEAD_translate);


	//BACK LEFT LEG TRANSFORMS
	glm::mat4 UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-6,2.4f));
	deer.addTransform("UBL", UBL_translate);
	glm::mat4 UBL_rotate = glm::rotate(glm::mat4(1.0f), cos(deer.movement_parameter*0.006f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UBL", UBL_rotate);
	UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,6,-2.4f));
	deer.addTransform("UBL", UBL_translate);

	glm::mat4 LBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-2.8f,3.0f));
	deer.addTransform("LBL", LBL_translate);
	glm::mat4 LBL_rotate = glm::rotate(glm::mat4(1.0f), sin(deer.movement_parameter*0.006f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LBL", LBL_rotate);
	LBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,2.8f,-3.0f));
	deer.addTransform("LBL", LBL_translate);
	

	//BACK RIGHT LEG TRANSFORMS
	glm::mat4 UBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-6,2.4f));
	deer.addTransform("UBR", UBR_translate);
	glm::mat4 UBR_rotate = glm::rotate(glm::mat4(1.0f), sin(deer.movement_parameter*0.006f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UBR", UBR_rotate);
	UBL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,6,-2.4f));
	deer.addTransform("UBR", UBL_translate);
	
	glm::mat4 LBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-2.8f,3.0f));
	deer.addTransform("LBR", LBR_translate);
	glm::mat4 LBR_rotate = glm::rotate(glm::mat4(1.0f), cos(deer.movement_parameter*0.006f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LBR", LBR_rotate);
	LBR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,2.8f,-3.0f));
	deer.addTransform("LBR", LBR_translate);

	//FRONT LEFT LEG TRANSFORMS
	glm::mat4 UFL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-5,-2.4f));
	deer.addTransform("UFL", UFL_translate);
	glm::mat4 UFL_rotate = glm::rotate(glm::mat4(1.0f), sin(deer.movement_parameter*0.007f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UFL", UFL_rotate);
	UFL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,5,2.4f));
	deer.addTransform("UFL", UFL_translate);

	glm::mat4 LFL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-2.2f,-2.4f));
	deer.addTransform("LFL", LFL_translate);
	glm::mat4 LFL_rotate = glm::rotate(glm::mat4(1.0f), cos(deer.movement_parameter*0.007f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LFL", LFL_rotate);
	LFL_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,2.2f,2.4f));
	deer.addTransform("LFL", LFL_translate);
	
	//FRONT RIGHT LEG TRANSFORMS
	glm::mat4 UFR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-5,-2.4f));
	deer.addTransform("UFR", UFR_translate);
	glm::mat4 UFR_rotate = glm::rotate(glm::mat4(1.0f), cos(deer.movement_parameter*0.007f) * 0.5f, glm::vec3(1,0,0));
	deer.addTransform("UFR", UFR_rotate);
	UFR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,5,2.4f));
	deer.addTransform("UFR", UFR_translate);
	
	glm::mat4 LFR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,-2.25f,-2.6f));
	deer.addTransform("LFR", LFR_translate);
	glm::mat4 LFR_rotate = glm::rotate(glm::mat4(1.0f), sin(deer.movement_parameter*0.007f) * 0.8f, glm::vec3(1,0,0));
	deer.addTransform("LFR", LFR_rotate);
	LFR_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0,2.25f,2.6f));
	deer.addTransform("LFR", LFR_translate);
	
	//MOVE DEER TO INITIAL POS
	glm::mat4 DEER_move = glm::translate(glm::mat4(1.0f), glm::vec3(deer.x_pos, 0.0f, deer.z_pos));
	deer.addTransform("Body", DEER_move);	
	
	deer.compileTransforms();
	deer.Draw(deer_shader);
	deer.resetTransforms();
	glPopMatrix(); 		// Don't forget to pop the Matrix
}

void updateScene() {

	int time_now = glutGet(GLUT_ELAPSED_TIME);
	delta_time = time_now - last_time;
	model_pos_z += delta_time;
    last_time = time_now;
	glutPostRedisplay();
}

void process_crowd(){
	for(int i=0; i<crowd_size; i++){
		deer_crowd[i].movement_parameter = last_time + 100*i;
		animate_deer(deer_crowd[i]);
	}	
}



void display() {
	//stbi_set_flip_vertically_on_load(true);
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height); 
	
	city_shader.use();
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
	city_shader.setMat4("projection", projection);
    city_shader.setMat4("view", view);
	glm::vec3 view_position = camera.GetViewPosition();
    city_shader.setVec3("view_position", view_position);
    city_shader.setVec3("light_position", light_position_world);
	city.Draw(city_shader);


	deer_shader.use();
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
	deer_shader.setMat4("projection", projection);
    deer_shader.setMat4("view", view);
	view_position = camera.GetViewPosition();
    deer_shader.setVec3("view_position", view_position);
    deer_shader.setVec3("light_position", light_position_world);

	process_crowd();
	glutSwapBuffers();
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
	if (key == 'q')
	{
		camera.ProcessKeyboard(UP, delta_time * 0.01f);
	}
	if (key == 'e')
	{
		camera.ProcessKeyboard(DOWN, delta_time * 0.01f);
	}
	if (key == 'z')
	{
		camera.ProcessKeyboard(ROTATE_LEFT, delta_time * 0.01f);
	}
	if (key == 'x')
	{
		camera.ProcessKeyboard(ROTATE_RIGHT, delta_time * 0.01f);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Climate Change Visualisation");

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

// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define PI 3.141592f
void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);

void create_desk();

GLuint	program;

GLint	loc_a_vertex;
GLint	loc_u_pvm_matrix;

glm::mat4 mat_PVM;

Object	g_desk, g_fan, g_sofa, g_tv;  // furniture
Camera	g_camera;				// viewer (you)

static float fan_rotate_degree=0.0f;

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);  
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);  
  glutCreateWindow("Modeling & Navigating Your Studio");
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK) 
    {
    std::cerr << "failed to initialize glew" << std::endl;
	return -1;
    }
  	
  init();
  
  glutMainLoop();

  return 0;
}

void init()
{
  g_desk.load_simple_obj("./data/desk.obj");
  g_fan.load_simple_obj("./data/fan.obj");
  g_sofa.load_simple_obj("./data/sofa.obj");
  g_tv.load_simple_obj("./data/tv.obj");
	
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // for wireframe rendering  

  	/* 서버쪽의 변수들을 받아오는 역할, 일종의 포인터  */
	program = Shader::create_program("./shader/simple.vert", "./shader/simple.frag");
	
	loc_u_pvm_matrix	= glGetUniformLocation(program, "u_pvm_matrix");
	
	loc_a_vertex	 = glGetAttribLocation(program, "a_vertex");

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	
  	//씨피유에서 미리 컴파일한 로직을 지피유가 프로그래밍을 익스큐선
	glUseProgram(program);

	// Camera setting
	glm::mat4   mat_Proj, mat_View, mat_Model;
	glm::mat4 rotate_matrix;
	
	// camera extrinsic param
	mat_View = glm::lookAt(
		g_camera.position(),				// eye position
		g_camera.center_position(), 		// center position
		g_camera.up_direction()			// up direction 
		);
	// camera intrinsic param
	mat_Proj = glm::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

	//draw desk
	mat_Model = glm::mat4(1.0);
	mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 0, 0));
	mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	mat_PVM = mat_Proj*mat_View*mat_Model;
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
	g_desk.draw(loc_a_vertex);

	//draw fan
	mat_Model = glm::mat4(1.0);
	mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
	mat_Model *= glm::rotate(fan_rotate_degree,glm::vec3(0,1,0));
	mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	mat_PVM = mat_Proj*mat_View*mat_Model;
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
	g_fan.draw(loc_a_vertex);

	//draw sofa
	mat_Model = glm::mat4(1.0);
	mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 5));
	mat_Model *= glm::rotate(PI,glm::vec3(0,1,0));
	mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	mat_PVM = mat_Proj*mat_View*mat_Model;
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
	g_sofa.draw(loc_a_vertex);

	//draw tv
	mat_Model = glm::mat4(1.0);
	mat_Model *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
	mat_Model *= glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	mat_PVM = mat_Proj*mat_View*mat_Model;
	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
	g_tv.draw(loc_a_vertex);

	glUseProgram(0);
	Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key){
		case 'a':
		case 'A':
			g_camera.rotate_left(0.2f);
			break;
		case 'd':
		case 'D':
			g_camera.rotate_right(-0.2f);
			break;
	}
 	
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch(key){

		case GLUT_KEY_LEFT:
			g_camera.move_left(-0.8);
			break;
		case GLUT_KEY_RIGHT:
			g_camera.move_right(-0.8);
			break;
		case GLUT_KEY_UP:
			g_camera.move_forward(-0.8);
			break;
		case GLUT_KEY_DOWN:
			g_camera.move_backward(-0.8);
			break;
		case GLUT_KEY_HOME:
			
			break;
	}
	glutPostRedisplay();
}

void idle()
{
  // TODO: update your variables in idle time
	if(fan_rotate_degree==360){
		fan_rotate_degree = 0;
	}
	fan_rotate_degree +=0.1f;
  glutPostRedisplay();
}

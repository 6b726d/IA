#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <string>
#include <vector>
#include <list>

#include <GL/glut.h>

#include "graph.hpp"
#include "tsp.hpp"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

//Globals
Graph g(20);
TSP e(20,20,20000,1,1,1);
vector<int> chromosome;

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//Begin Special Functions

//End Special Functions

GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
	    cout<<"X: "<<x<<endl;
        cout<<"Y: "<<y<<endl;
	}
}

//function called on each frame
GLvoid window_idle();

//Begin Main Function

//End Main Functions

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL: TSP");


	initGL();
	init_scene();

	//Begin Objects
	g.GenerateGraph();
	e.Creation();
	chromosome = e.Evolution(g);
	//End Objects

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutMouseFunc(&callback_mouse);

	glutKeyboardFunc(&window_key);

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

GLvoid initGL() {
	glClearColor(RED, GREEN, BLUE, ALPHA);
}

//Begin Functions

//End Functions

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glViewport(0,0,400,400);
	glOrtho(-200.0f, 200.0f, -200.0f, 200.0f, -1.0f, 1.0f);

	//Dibujar
	//g.DrawGraph();
	e.Draw(g,chromosome);

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200.0f, 200.0f, -200.0f, 200.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}

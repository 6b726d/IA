#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <queue>
#include"point.h"
#define KEY_ESC 27
graph G(150, 100, 5);

void displayGizmo()
{
    glPointSize(3.0);
    glColor3f(70.3, 0.3, 0.3);
	for(int i = 0; i < G.points.size(); i++){
        glBegin(GL_POINTS);
		glColor3f(70.3, 0.3, 0.3);
        glVertex2i(G.points[i].x, G.points[i].y);
        glEnd();
        for(int j = 0; j < G.points[i].edges.size();j++){
            point dest_point;
            dest_point = G.points[G.points[i].edges[j].next_point];
			glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            glVertex2f(G.points[i].x, G.points[i].y);
            glVertex2f(dest_point.x, dest_point.y);
            glEnd();
        }
	}
}

void drawPath()
{
	glPointSize(15.0);
	glColor3f(0.8, 1, 0); //0.8/1/0
	for(int i = 0; i < G.pathFound.size(); i++)
	{
		glBegin(GL_POINTS);
		glColor3f(0.8, 1, 0); //0.8/1/0
		glVertex2i(G.pathFound[i].x, G.pathFound[i].y);
		glEnd();
		/*for(int j = 0; j < G.points[i].edges.size();j++)
		{
			point dest_point;
			dest_point = G.points[G.points[i].edges[j].next_point];
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			glVertex2f(G.points[i].x, G.points[i].y);
			glVertex2f(dest_point.x, dest_point.y);
			glEnd();
		}*/
	}
	glPointSize(10.0);
	glColor3f(0.9, 0, 0.2); //0.9/0/0.2
	for(int i = 0; i < G.path2.size(); i++)
	{
		glBegin(GL_POINTS);
		glColor3f(0.9, 0, 0.2); //0.9/0/0.2
		glVertex2i(G.path2[i].x, G.path2[i].y);
		glEnd();
		/*for(int j = 0; j < G.points[i].edges.size();j++)
		{
		point dest_point;
		dest_point = G.points[G.points[i].edges[j].next_point];
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(G.points[i].x, G.points[i].y);
		glVertex2f(dest_point.x, dest_point.y);
		glEnd();
		}*/
	}
	
}

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(0.0f,  100.0f,0.0f, 100.0f, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())
	drawPath();
	//dibuja el gizmo
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana
	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);

	G.AStar(0,5);
	G.ciega(0,5);
	
	
	
	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}

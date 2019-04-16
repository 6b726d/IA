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

#include "quad.hpp"
#include "token.hpp"
#include "ntree.hpp"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

//Globals
vector<string> board(8);    //Board
list<Quad*> quads;          //Quads
list<Token*> tokens0;       //Player
list<Token*> tokens1;       //AI
int clicks = 0;             //Number of Clicks
vector<double> point0(2);   //Move Point 0
vector<double> point1(2);   //Move Point 1
vector<int> pos0(2);        //Move Position 0
vector<int> pos1(2);        //Move Position 1
int ht = 5;                 //NTree Height
int turn = 0;               //0:Player, 1:IA, 2:EndGame

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//Begin Special Functions
void MoveToken() {
    char attack = 'n';  //n:null, l:left, r:right
    int ipos = -1;      //Attack Quad
    int jpos = -1;      //Attack Quad
    double xt = 0.0;    //Attack Token1
    double yt = 0.0;    //Attack Token1
    list<Token*>::iterator itT;
    for (itT = tokens0.begin(); itT != tokens0.end(); itT++) {
        if ( ( (point0[0] >= (*itT)->getX()-12) && (point0[0] <= (*itT)->getX()+12) ) &&
             ( (point0[1] >= (*itT)->getY()-12) && (point0[1] <= (*itT)->getY()+12) ) )
        {
            //Case Simple
            if ( (pos0[0] == pos1[0]+1) && ( (pos0[1] == pos1[1]+1) || (pos0[1] == pos1[1]-1) ) )
            {
                (*itT)->Move(point1[0],point1[1]);
                board[pos0[0]][pos0[1]] = '+';
                board[pos1[0]][pos1[1]] = '0';
            }
            //Case Attack Left
            else if ( (board[pos0[0]-1][pos0[1]-1] == board[pos1[0]+1][pos1[1]+1]) && (board[pos0[0]-1][pos0[1]-1] == '1') )
            {
                //Only One Token
                if ( (pos0[0] == pos1[0]+2) && (pos0[1] == pos1[1]+2) )
                {
                    (*itT)->Move(point1[0],point1[1]);
                    board[pos0[0]][pos0[1]] = '+';
                    board[pos1[0]][pos1[1]] = '0';
                    board[pos0[0]-1][pos0[1]-1] = '+';//Delete
                    //I'm not Sure
                    attack = 'l';
                    ipos = pos0[0]-1;
                    jpos = pos0[1]-1;
                    xt = point0[0] - 50;
                    yt = point0[1] + 50;
                }
            }
            //Case Attack Right
            else if ( (board[pos0[0]-1][pos0[1]+1] == board[pos1[0]+1][pos1[1]-1]) && (board[pos0[0]-1][pos0[1]+1] == '1') )
            {
                //Only One Token
                if ( (pos0[0] == pos1[0]+2) && (pos0[1] == pos1[1]-2) )
                {
                    (*itT)->Move(point1[0],point1[1]);
                    board[pos0[0]][pos0[1]] = '+';
                    board[pos1[0]][pos1[1]] = '0';
                    board[pos0[0]-1][pos0[1]+1] = '+';//Delete
                    //I'm not Sure
                    attack = 'r';
                    ipos = pos0[0]-1;
                    jpos = pos0[1]+1;
                    xt = point0[0] + 50;
                    yt = point0[1] + 50;
                }
            }

        }
    }
    list<Quad*>::iterator itQ;
    for (itQ = quads.begin(); itQ != quads.end(); itQ++) {
        if ( ((*itQ)->getI() == pos0[0]) && ((*itQ)->getJ() == pos0[1]) ) {
            (*itQ)->setType('+');
        }
        else if ( ((*itQ)->getI() == pos1[0]) && ((*itQ)->getJ() == pos1[1]) ) {
            (*itQ)->setType('0');
        }
        else if ( (attack != 'n') && (((*itQ)->getI() == ipos) && ((*itQ)->getJ() == jpos)) ) {
            (*itQ)->setType('+');
        }
    }
    //Delete Token1
    if (attack != 'n') {
        list<Token*>::iterator itD;
        for (itD = tokens1.begin(); itD != tokens1.end(); itD++) {
            if ( ( (xt >= (*itD)->getX()-12) && (xt <= (*itD)->getX()+12) ) &&
                 ( (yt >= (*itD)->getY()-12) && (yt <= (*itD)->getY()+12) ) )
            {
                delete(*itD);
                itD = tokens1.erase(itD);
            }
        }
    }
}

void MoveAI() {
    NTree T;
    T.max_height = ht;
    T.create_tree('1','0',board);
    bool attack = false;
    vector<vector<int>> bpos = T.MinMax();
    cout<<"IA Pos0.i: "<<bpos[0][0]<<endl;
    cout<<"IA Pos0.j: "<<bpos[0][1]<<endl;
    cout<<"IA Pos1.i: "<<bpos[1][0]<<endl;
    cout<<"IA Pos1.j: "<<bpos[1][1]<<endl;
    cout<<"IA Pos2.i: "<<bpos[2][0]<<endl;
    cout<<"IA Pos2.j: "<<bpos[2][1]<<endl;
    board[bpos[0][0]][bpos[0][1]] = '+'; //Pos0
    board[bpos[1][0]][bpos[1][1]] = '1'; //Pos1
    if ((bpos[2][0] != -1) && (bpos[2][1] != -1)) {
        board[bpos[2][0]][bpos[2][1]] = '+'; //Delete Token
        attack = true;
    }
    vector<double> point(2);
    vector<vector<double>> points(3);
    point[0] = 300; //-200,200 => Available
    point[1] = 300; //-200,200 => Available
    points[0] = point;
    points[1] = point;
    points[2] = point;
    //Quads
    cout<<"IA QUADS"<<endl;
    list<Quad*>::iterator itQ;
    for (itQ = quads.begin(); itQ != quads.end(); itQ++) {
        if ( ((*itQ)->getI() == bpos[0][0]) && ((*itQ)->getJ() == bpos[0][1]) ) {
            cout<<"IA POS0"<<endl;
            (*itQ)->setType('+');
            point[0] = (*itQ)->getX() - 200;
            point[1] = -((*itQ)->getY() - 200);
            points[0] = point;
        }
        else if ( ((*itQ)->getI() == bpos[1][0]) && ((*itQ)->getJ() == bpos[1][1]) ) {
            cout<<"IA POS1"<<endl;
            (*itQ)->setType('1');
            point[0] = (*itQ)->getX() - 200;
            point[1] = -((*itQ)->getY() - 200);
            points[1] = point;
        }
        else if ( (attack == true) && (((*itQ)->getI() == bpos[2][0]) && ((*itQ)->getJ() == bpos[2][1])) ) {
            cout<<"IA POS2"<<endl;
            (*itQ)->setType('+');
            point[0] = (*itQ)->getX() - 200;
            point[1] = -((*itQ)->getY() - 200);
            points[2] = point;
        }
    }
    //Tokens
    cout<<"IA TOKENS"<<endl;
    list<Token*>::iterator itT;
    for (itT = tokens1.begin(); itT != tokens1.end(); itT++) {
        //Move
        if ( ((*itT)->getX() == points[0][0]) && ((*itT)->getY() == points[0][1]) ) {
            cout<<"IA Move"<<endl;
            (*itT)->Move(points[1][0],points[1][1]);
        }
    }
    list<Token*>::iterator itD;
    for (itD = tokens0.begin(); itD != tokens0.end(); itD++) {
        //Delete
        if ( (attack == true) && (((*itD)->getX() == points[2][0]) && ((*itD)->getY() == points[2][1])) ) {
            cout<<"IA Attack"<<endl;
            delete(*itD);
            itD = tokens0.erase(itD);
        }
    }
}
//End Special Functions

GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
	    if (turn == 0) {
            list<Quad*>::iterator itQ;
            for (itQ = quads.begin(); itQ != quads.end(); itQ++)
            {
                if ( ( (x >= (*itQ)->getX()-24) && (x <= (*itQ)->getX()+24) ) &&
                     ( (y >= (*itQ)->getY()-24) && (y <= (*itQ)->getY()+24) ) )
                {
                    if (clicks == 0) {
                        if ((*itQ)->getType() == '0') {
                            //Window
                            point0[0] = x-200;
                            point0[1] = -(y-200);
                            pos0[0] = (*itQ)->getI();
                            pos0[1] = (*itQ)->getJ();
                            clicks = clicks + 1;
                        }
                    }
                    else if (clicks == 1)
                    {
                        if ((*itQ)->getType() == '+') {
                            //Window
                            point1[0] = (*itQ)->getX() - 200;
                            point1[1] = -( (*itQ)->getY() - 200 );
                            pos1[0] = (*itQ)->getI();
                            pos1[1] = (*itQ)->getJ();
                            MoveToken();
                            clicks = 0;
                            glutPostRedisplay();//
                            turn = 1;//
                        }
                    }
                    cout<<"Type: "<<(*itQ)->getType()<<endl;
                }
            }
            cout<<"X: "<<x<<endl;
            cout<<"Y: "<<y<<endl;
            cout<<"Clicks: "<<clicks<<endl;
	    }
	    else if (turn == 1) {
            cout<<"IA Begin"<<endl;
            MoveAI();
            turn = 0;
            cout<<"IA End"<<endl;
	    }
	}
}

//function called on each frame
GLvoid window_idle();

//Begin Main Function
void GenerateBoard() {
    board[0] = "-1-1-1-1";
    board[1] = "1-1-1-1-";
    board[2] = "-1-1-1-1";
    board[3] = "+-+-+-+-";
    board[4] = "-+-+-+-+";
    board[5] = "0-0-0-0-";
    board[6] = "-0-0-0-0";
    board[7] = "0-0-0-0-";
}

void GenerateQuads() {
    double x = 25.0;
    double y = 25.0;
    for(int i=0; i<board.size(); i++) {
        for(int j=0; j<board[i].size(); j++) {
            quads.push_back(new Quad(x,y,i,j,board[i][j]));
            x = x + 50.0;
        }
        x = 25.0;
        y = y + 50.0;
    }
}

void GenerateTokens() {
    double x0 = 0.0;
	double y0 = 0.0;
	double x1 = 0.0;
	double y1 = 0.0;

	//
	x0 = -175.0;
	y0 = -175.0;
	x1 = -125.0;
	y1 = 175.0;
	for (int i=1; i<=4; i++) {
	    tokens0.push_back(new Token(x0,y0,0,0));
        tokens1.push_back(new Token(x1,y1,1,0));
        x0 = x0 + 100.0;
        x1 = x1 + 100.0;
	}
	x0 = -125.0;
	y0 = -125.0;
	x1 = -175.0;
	y1 = 125.0;
	for (int i=1; i<=4; i++) {
	    tokens0.push_back(new Token(x0,y0,0,0));
        tokens1.push_back(new Token(x1,y1,1,0));
        x0 = x0 + 100.0;
        x1 = x1 + 100.0;
	}
	x0 = -175.0;
	y0 = -75.0;
	x1 = -125.0;
	y1 = 75.0;
	for (int i=1; i<=4; i++) {
	    tokens0.push_back(new Token(x0,y0,0,0));
        tokens1.push_back(new Token(x1,y1,1,0));
        x0 = x0 + 100.0;
        x1 = x1 + 100.0;
	}
}
//End Main Functions

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL: Checkers");


	initGL();
	init_scene();

	//Begin Objects
	GenerateBoard();
	GenerateQuads();
	GenerateTokens();
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

//Board
void DisplayBoard()
{
    double x = 0.0;
    double y = 0.0;
    //
    glColor3d(1.0,1.0,1.0);
    x = -175.0;
    y = 175.0;
    for (int i=0; i<33; i++) {
        glBegin(GL_QUADS);
        glVertex3d(x-25,y-25,0.0);
        glVertex3d(x+25,y-25,0.0);
        glVertex3d(x+25,y+25,0.0);
        glVertex3d(x-25,y+25,0.0);
        glEnd();
        x = x + 100.0;
        if (i == 4) {
            x = -125.0;
            y = 125.0;
        }
        else if (i == 8) {
            x = -175.0;
            y = 75.0;
        }
        else if (i == 12) {
            x = -125.0;
            y = 25.0;
        }
        else if (i == 16) {
            x = -175.0;
            y = -25.0;
        }
        else if (i == 20) {
            x = -125.0;
            y = -75.0;
        }
        else if (i == 24) {
            x = -175.0;
            y = -125.0;
        }
        else if (i == 28) {
            x = -125.0;
            y = -175.0;
        }
    }
}
void DisplayTokens()
{
    list<Token*>::iterator it0;
    for (it0 = tokens0.begin(); it0 != tokens0.end(); it0++) {
        (*it0)->Draw();
    }
    list<Token*>::iterator it1;
    for (it1 = tokens1.begin(); it1 != tokens1.end(); it1++) {
        (*it1)->Draw();
    }
}

//
GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glViewport(0,0,400,400);
	glOrtho(-200.0f, 200.0f, -200.0f, 200.0f, -20.0f, 20.0f);

	//Dibujar
	DisplayBoard();
	DisplayTokens();

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200.0f, 200.0f, -200.0f, 200.0f, -20.0f, 20.0f);

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

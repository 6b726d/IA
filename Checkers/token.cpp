#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>

#include "token.hpp"

using namespace std;

Token::Token(double _x, double _y, bool _player, bool _type) {
    x = _x;
    y = _y;
    player = _player;
    type = _type;
}

double Token::getX() {
    return x;
}

double Token::getY() {
    return y;
}

bool Token::getPlayer() {
    return player;
}

bool Token::getType() {
    return type;
}

void Token::setX(double nX) {
    x = nX;
}

void Token::setY(double nY) {
    y = nY;
}

void Token::setPlayer(bool nPlayer) {
    player = nPlayer;
}

void Token::setType(bool nType) {
    type = nType;
}

void Token::Draw() {
    glPushMatrix();
    if (player == 0) {
        glColor3d(1.0,0.0,0.0);
    }
    else {
       glColor3d(0.0,0.0,1.0);
    }
    glTranslated(x,y,0.0);
    glutSolidCube(25.0);
    glPopMatrix();
}

void Token::Move(double nX, double nY) {
    x = nX;
    y = nY;
}

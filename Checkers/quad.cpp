#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>

#include "quad.hpp"

using namespace std;

Quad::Quad(double _x, double _y, int _i, int _j, char _type) {
    x = _x;
    y = _y;
    i = _i;
    j = _j;
    type = _type;
}

double Quad::getX() {
    return x;
}

double Quad::getY() {
    return y;
}

int Quad::getI() {
    return i;
}

int Quad::getJ() {
    return j;
}

char Quad::getType() {
    return type;
}

void Quad::setX(double nX) {
    x = nX;
}

void Quad::setY(double nY) {
    y = nY;
}

void Quad::setI(double nI) {
    i = nI;
}

void Quad::setJ(double nJ) {
    j = nJ;
}

void Quad::setType(char nType) {
    type = nType;
}

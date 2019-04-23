#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <chrono>
#include <random>

#include <GL/glut.h>

#include "graph.hpp"
#include "functions.hpp"

using namespace std;

Graph::Graph(int _n) {
    n = _n;
}

void Graph::GenerateGraph() {
    //Points
    Point p;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(-200.0,200.0);
    for(int i=0; i<n; i++) {
        p.x = distribution(generator);
        p.y = distribution(generator);
        nodes.push_back(p);
    }
    //Distances
    vector<double> vec(n);
    double d;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if (i==j) {
                d = 0;
            }
            else {
                d = EDistance(nodes[i].x,nodes[j].x,nodes[i].y,nodes[j].y);
            }
            vec[j] = d;
        }
        edges.push_back(vec);
    }
}

void Graph::DrawGraph() {
    //Draw Points
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i=0; i<n; i++) {
        glVertex2d(nodes[i].x,nodes[i].y);
    }
    glEnd();
    //Draw Lines
    glBegin(GL_LINES);
    for(int i=0; i<n; i++) {
        for(int j=i+1; j<n; j++) {
            glVertex2d(nodes[i].x,nodes[i].y);
            glVertex2d(nodes[j].x,nodes[j].y);
        }
    }
    glEnd();
}

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>

#include <chrono>
#include <algorithm>
#include <random>
#include <limits>

#include "tsp.hpp"
#include "graph.hpp"
#include "functions.hpp"

using namespace std;

TSP::TSP(int _n, int _population, int _generations, int _selection, int _crossover, int _mutation) {
    n = _n;
    population = _population;
    generations = _generations;
    selection = _selection;
    crossover = _crossover;
    mutation = _mutation;
}

void TSP::Creation() {
    //Sequential Path Creation Begin
    vector<int> chromosome;
    for(int i=0; i<n; i++) {
        chromosome.push_back(i);
    }
    chromosome.push_back(0);
    //Sequential Path Creation End

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0,n-1);
    for(int i=0; i<population; i++) {
        shuffle(chromosome.begin()+1,chromosome.end()-1,generator);
        individuals.push_back(chromosome);
        values.push_back(0.0);
    }
}

void TSP::Evaluate(Graph g) {
    double s;
    int ipos;
    int jpos;
    for(int i=0; i<population; i++) {
        s = 0.0;
        for(int j=0; j<n; j++) {
            ipos = individuals[i][j];
            jpos = individuals[i][j+1];
            s = s + g.edges[ipos][jpos];
        }
        values[i] = s;
    }
}

vector<int> TSP::Selection() {
    vector<int> parents(2);
    ///Tournament
    vector<double> cval(values);
    int p1 = min_element(cval.begin(),cval.end()) - cval.begin();
    cval[p1] = numeric_limits<double>::max();
    int p2 = min_element(cval.begin(),cval.end()) - cval.begin();
    cval[p2] = numeric_limits<double>::max();
    parents[0] = p1;
    parents[1] = p2;
    return parents;
}

vector<int> TSP::Crossover(vector<int> parents) {
    vector<int> parent1(individuals[parents[0]]);
    vector<int> parent2(individuals[parents[1]]);
    if (crossover == 0) {      ///PMX (Partially Mapped)
        //parent1 < parent2
        int pos;
        vector<int> idxs0;
        vector<int> idxs1;
        vector<int> idxs2;
        //
        vector<int> chromosome(parent1);
        vector<bool> idxs(n+1);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        uniform_int_distribution<int> distribution(1,n-1);
        int pos1 = distribution(generator);
        int pos2 = distribution(generator);
        if (pos2 < pos1) {
            swap(pos1,pos2);
        }
        ///Step1
        for(int i=pos1; i<=pos2; i++) {
            idxs[i] = 1; //true
            pos = find(parent1.begin(),parent1.end()-1,parent2[i]) - parent1.begin();
            if ( (pos >= pos1) && (pos <= pos2) ) {
                idxs0.push_back(pos);
            }
            else {
                idxs2.push_back(i);
            }
        }
        for(int i=pos1; i<=pos2; i++) {
            pos = find(idxs0.begin(),idxs0.end(),i) - idxs0.begin();
            if ( (pos < 0) || (pos >= idxs0.size()) ) {
                idxs1.push_back(i);
            }
        }
        ///Step2
        for(int i=0; i<idxs1.size(); i++) {
            pos = find(parent2.begin(),parent2.end()-1,parent1[idxs1[i]]) - parent2.begin();
            chromosome[pos] = parent2[idxs2[i]];
            idxs[pos] = 1; //true
        }
        ///Step 3
        for(int i=0; i<n+1; i++) {
            if (idxs[i] == 0) {
                chromosome[i] = parent2[i];
            }
        }
        return chromosome;
    }
    else {                          ///Edge Recombination
        ///Creation Table Begin
        int csize;
        vector<int> sizes;
        ///-1:+
        vector<vector<int>> table;
        vector<int> edges4(4);
        int pos1,pos2;
        int e1,e2,e3,e4;
        for(int i=0; i<n; i++) {
            pos1 = find(parent1.begin(),parent1.end()-1,i) - parent1.begin();
            if (pos1 == 0) {
                e1 = parent1[n-1];
                e2 = parent1[pos1+1];
            }
            else if (pos1 == n-1) {
                e1 = parent1[pos1-1];
                e2 = parent1[0];
            }
            else {
                e1 = parent1[pos1-1];
                e2 = parent1[pos1+1];
            }
            pos2 = find(parent2.begin(),parent2.end()-1,i) - parent2.begin();
            if (pos2 == 0) {
                e3 = parent2[n-1];
                e4 = parent2[pos2+1];
            }
            else if (pos2 == n-1) {
                e3 = parent2[pos2-1];
                e4 = parent2[0];
            }
            else {
                e3 = parent2[pos2-1];
                e4 = parent2[pos2+1];
            }
            edges4[0] = e1;
            edges4[1] = e2;
            edges4[2] = e3;
            edges4[3] = e4;
            sort(edges4.begin(),edges4.end());
            ///-1
            csize = 4;
            for(int j=0; j<3; j++) {
                if (edges4[j+1] == edges4[j]) {
                    edges4[j+1] = -1;
                    csize = csize - 1;
                }
            }
            table.push_back(edges4);
            sizes.push_back(csize);
        }
        ///Creation Table End
        vector<int> chromosome;
        chromosome.push_back(0);
        int idx = 0;
        int pos = 0;
        int current_element = 0;
        int imin = 0;
        int smin = 0;
        DeleteElementPlus(table,sizes,current_element);
        for(int i=1; i<n; i++) {
            //Common Edge
            pos = find(table[idx].begin(),table[idx].end(),-1) - table[idx].begin();
            if ( (table[idx].size() != 0) && (pos >= 0) && (pos < table[idx].size()) ) {
                current_element = table[idx][pos-1];
                DeleteElementPlus(table,sizes,current_element);
                idx = current_element;
            }
            else {
                imin = table[idx][0];
                smin = sizes[imin];
                current_element = imin;
                for(int j=1; j<table[idx].size(); j++) {
                    imin = table[idx][j];
                    if (smin > sizes[imin]) {
                        smin = sizes[imin];
                        current_element = imin;
                    }
                }
                DeleteElementPlus(table,sizes,current_element);
                idx = current_element;
            }
            chromosome.push_back(idx);
        }
        chromosome.push_back(0);
        return chromosome;
    }
}

void TSP::Mutation(int p) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(1,n-1);
    int pos1 = distribution(generator);
    int pos2 = distribution(generator);
    if (pos2 < pos1) {
        swap(pos1,pos2);
    }
    if (mutation == 0) {            ///Insert
        individuals[p].insert(individuals[p].begin()+pos1,individuals[p][pos2]);
        individuals[p].erase(individuals[p].begin()+pos2+1);
    }
    else if (mutation == 1) {       ///Swap
        swap(individuals[p][pos1],individuals[p][pos2]);
    }
    else if (mutation == 2) {       ///Inverse
        reverse(individuals[p].begin()+pos1,individuals[p].begin()+pos2);
    }
    else {                          ///Scramble
        shuffle(individuals[p].begin()+pos1,individuals[p].begin()+pos2,generator);
    }
}

vector<int> TSP::Evolution(Graph g) {
    cout<<"Initial Chromosomes"<<endl;
    PrintIVector2D(individuals);
    Evaluate(g);
    cout<<"Initial Values"<<endl;
    PrintDVector1D(values);
    vector<int> parents;
    vector<int> best_chromosome;
    for(int i=0; i<generations; i++) {
        parents = Selection();
        for(int c=0; c<population; c++) {
            if ( (c != parents[0]) && (c != parents[1]) ) {
                individuals[c] = Crossover(parents);
                Mutation(c);
            }
        }
        Evaluate(g);
    }
    cout<<"Final Chromosomes"<<endl;
    PrintIVector2D(individuals);
    cout<<"Final Values"<<endl;
    PrintDVector1D(values);
    int p = min_element(values.begin(),values.end()) - values.begin();
    cout<<"Best Chromosome: "<<endl;
    PrintIVector1D(individuals[p]);
    cout<<"Value: "<<values[p]<<endl;
    return individuals[p];
}

void TSP::Draw(Graph g, vector<int> chromosome) {
    //Draw Points
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i=0; i<n; i++) {
        glVertex2d(g.nodes[i].x,g.nodes[i].y);
    }
    glEnd();
    //Draw Lines
    int p;
    int q;
    glBegin(GL_LINES);
    for(int i=0; i<n; i++) {
        p = chromosome[i];
        q = chromosome[i+1];
        glVertex2d(g.nodes[p].x,g.nodes[p].y);
        glVertex2d(g.nodes[q].x,g.nodes[q].y);
    }
    glEnd();
}

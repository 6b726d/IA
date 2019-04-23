#include <cmath>
#include <algorithm>
#include <iostream>
#include "functions.hpp"

using namespace std;

void DeleteElementPlus(vector<vector<int>> &table, vector<int> &sizes, int e) {
    int pos = 0;
    bool del = false;
    for(int i=0; i<table.size(); i++) {
        if (table[i].size() > 0) {
            pos = find(table[i].begin(),table[i].end(),e) - table[i].begin();
            if ( (pos >= 0) && (pos < table[i].size()-1)) {
                if (table[i][pos+1] == -1) {
                    del = true;
                }
            }
            if ((pos >= 0) && (pos < table[i].size())) {
                table[i].erase(table[i].begin() + pos);     //Element
                sizes[i] = sizes[i] - 1;
            }
            if (del == true) {
                table[i].erase(table[i].begin() + pos);     //-1
                del = false;
            }
        }
    }
}

void DeleteElement(vector<vector<int>> &table, int e) {
    for(int i=0; i<table.size(); i++) {
        table[i].erase(remove(table[i].begin(),table[i].end(),e),table[i].end());
    }
}

void PrintIVector1D(vector<int> vec) {
    for(int i=0; i<vec.size(); i++) {
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

void PrintIVector2D(vector<vector<int>> vec) {
    for(int i=0; i<vec.size(); i++) {
        for(int j=0; j<vec[i].size(); j++) {
            cout<<vec[i][j]<<" ";
        }
        cout<<endl;
    }
}

void PrintDVector1D(vector<double> vec) {
    for(int i=0; i<vec.size(); i++) {
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

void PrintDVector2D(vector<vector<double>> vec) {
    for(int i=0; i<vec.size(); i++) {
        for(int j=0; j<vec[i].size(); j++) {
            cout<<vec[i][j]<<" ";
        }
        cout<<endl;
    }
}

double EDistance(double x1, double x2, double y1, double y2) {
    double p = (x1-x2)*(x1-x2); //pow((x1-x2),2)
    double q = (y1-y2)*(y1-y2); //pow((y1-y2),2)
    double r = sqrt(p+q);
    return r;
}

#include <vector>

using namespace std;

class Graph;

class TSP {
    int n;                              //Size of Path
    vector<vector<int>> individuals;    //Paths
    vector<double> values;              //Distance of Path
    int population;                     //Number of Paths
    int generations;                    //Number of Iterations
    int selection;                      //Type of Selection
    int crossover;                      //Type of Crossover
    int mutation;                       //Type of Mutation
public:
    TSP(int _n, int _population, int _generations, int _selection, int _crossover, int _mutation);
    void Creation();
    void Evaluate(Graph g);
    vector<int> Selection();
    vector<int> Crossover(vector<int> parents);
    void Mutation(int p);
    vector<int> Evolution(Graph g);
    void Draw(Graph g, vector<int> chromosome);
};

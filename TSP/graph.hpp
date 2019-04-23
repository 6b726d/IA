#include <vector>

using namespace std;

struct Point {
    double x;
    double y;
};

class Graph {
    int n;                          //Size
    vector<Point> nodes;            //Points
    vector<vector<double>> edges;   //Distances
public:
    Graph(int _n);
    void GenerateGraph();
    void DrawGraph();
    friend class TSP;
};

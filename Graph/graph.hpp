#include<math.h>
#include<algorithm>
#include<stdlib.h>
#include<vector>
#include<list>
using namespace std;

typedef pair<float, int> iPair;

class edge{
public:
    int next_point;
    double value;
    edge(int p, double v){
        next_point = p;
        value = v;
    }
    edge(){
        next_point = 0;
        value = 0;
    }
    operator <(edge e){
        return value < e.value;
    }
};
class point{
    public:
    int x;
    int y;
    vector<edge> edges;
    point(int a, int b){
        x = a;
        y = b;
    }
    point(){
        x = 0;
        y = 0;
    }
    double distance(point p){
        return sqrt(pow(x-p.x,2) + pow(y-p.y,2));
    }
    operator =(point n){
        x = n.x;
        y = n.y;
    }
};

class graph{
    public:
		vector<vector <bool> > adj;
		int maxT;
		int total;
		vector<vector<pair<int,float> > > lista;
		vector<point> pathFound, path2;
		double matrix[150][150];
		list<int> *adj11;   
        vector<point> points;
        vector<edge> edges;
        graph(int total0, int limit, int edges_count){
			
			total = total0;
			adj11 = new list<int>[total0]; 
            for(int i = 0; i < total0; i++){
                int x = rand()%limit;
                int y = rand()%limit;
                point a(x,y);
                points.push_back(a);
            }
            for(int i = 0; i <points.size(); i++){
                for(int j = 0; j < points.size(); j++){
                    double dist = points[i].distance(points[j]);
                    edge new_edge(j, dist);
                    points[i].edges.push_back(new_edge);
                }
                sort(points[i].edges.begin(), points[i].edges.end());
                points[i].edges.resize(edges_count+1);
                points[i].edges.erase(points[i].edges.begin());
                cout<<"point : "<<i<<endl;
                for(int p = 0; p<points[i].edges.size();p++){
                    cout<<i<<": "<<points[i].edges[p].next_point<<" ";
                }
                cout<<endl;
                //points[i].edges.erase(points[i].edges.begin()+5);
            }

        }
		
		void generateMatrix()
		{
			for (int i=0; i<total; i++)
			{
				for (int j=0; j<total; j++)
				{
					matrix[i][j] = 0;
				}
			}
			
			for (int i=0; i<total; i++)
			{
				for (int j=0; j<points[i].edges.size(); j++)
				{
					matrix[i][points[i].edges[j].next_point] = points[i].edges[j].value;
				}				
			}
			
			/*for (int i=0; i<total; i++)
			{
				for (int j=0; j<total; j++)
				{
					cout<<matrix[i][j]<<" ";
				}
				cout<<endl;
			}*/
		}	
		
		void gen()
		{
			adj.resize(total);
			for (int i = 0; i < total; i++)
				adj[i].assign(total, false);
			
			cout<<"size adj "<<adj.size()<<endl;
			cout<<"size adj1 "<<adj[0].size()<<endl;
			
			
			
			lista.resize(total);
			
			
			for (int i=0; i<total; i++)
			{
				for (int j=0; j<points[i].edges.size(); j++)
				{
					lista[i].push_back(make_pair(points[i].edges[j].next_point, points[i].edges[j].value));
					adj[i][points[i].edges[j].next_point]=true;
					adj[points[i].edges[j].next_point][i]=true;
					adj11[i].push_back(points[i].edges[j].next_point);
				}				
			}
			
			for (int i=0; i<adj.size(); i++)
			{
				for (int j=0; j<adj[i].size(); j++)
				{
					cout<<adj[i][j]<<" ";
					//if (adj[i][j] != 0)
						//cout<<"first error"<<endl;
				}
				cout<<endl;
			}
			
		}
		
		int minDistance(double dist[], bool sptSet[]) 
		{ 
			double min = maxT;
			int min_index; 
			
			for (int v = 0; v < total; v++) 
				if (sptSet[v] == false && dist[v] <= min) 
					min = dist[v], min_index = v; 
			
			return min_index; 
		} 
		
		double heuristica(point ini, point fin)
		{
			return sqrt(pow(ini.x-fin.x,2) + pow(ini.y-fin.y,2));
		}
		
		void AStar(int src, int bus)
		{
			gen();
			cout<<"Salio de gen"<<endl;
			maxT = 1000000;
			priority_queue<iPair, vector <iPair> , greater<iPair> > pq;
			
			vector<double> dist(total, maxT);
			//vector<bool> vis(Vcnt, false);
			
			vector<int> predecesores(total,-1);
			
			pq.push(make_pair(0, src));
			dist[src] = 0;
			
			while (!pq.empty())
			{
				double u = pq.top().second;
				//vis[u] = true;
				pq.pop();
				
				for (int i=0; i < lista[u].size(); i++)
				{
					int v = lista[u][i].first;
					double weight = lista[u][i].second;
					
					if (dist[v] > dist[u] + weight)
					{
						dist[v] = dist[u] + weight;
						predecesores[v] = u;
						
						pq.push(make_pair(dist[v]+heuristica(points[u],points[v]), v));
					}
				}
			}
		
			printCamino(predecesores,src,bus);
		}
		
		void printCamino(vector<int> lista, int ini, int ult)
		{
			vector<int> camino;
			pathFound.clear();
			while(ini != ult)
			{
				//cout<<ult<<",";
				camino.push_back(ult);
				ult = lista[ult];
			}
			camino.push_back(ini);
			cout<<"Cantidad nodos: "<<camino.size()<<endl;
			cout<<"Camino desde "<<ini<<" hacia "<<ult<<": ";
			
			//cout<<ini<<endl;
			for (int i=camino.size()-1; i>=0; i--)
			{
				cout<<camino[i]<<",";
				pathFound.push_back(points[camino[i]]);
			}
			cout<<endl;
			//cout<<"GG "<<pathFound.size()<<endl;
		}
		
		void DFSUtil(int v, bool visited[]) 
		{ 
			visited[v] = true; 
			cout << v << " "; 
			
			for (int i = 0; i<lista[v].size(); i++)
			{
				if (!visited[i]) 
				{
					DFSUtil(i, visited); 
				}
			}
		} 
		
		void DFS(int v) 
		{ 
			bool *visited = new bool[total]; 
			for (int i = 0; i < total; i++) 
				visited[i] = false; 
			
			DFSUtil(v, visited); 
		}
		
		vector<int> reversed_topological_list;
		vector<bool> visitado;
		
		void check()
		{
			
		}
		
		int dfsPro(int u, int cur_time)
		{
			cout<<"GG total"<<endl;
			int start_time = cur_time;
			cur_time+=1;
			visitado[u]=true;
			for(int i = 0; i < adj[i].size() ;i++)
			{
				if (adj[u][i] == true)
				{
					if( visitado[i] == 0 )
					{
						cout<<"if"<<endl;
						cur_time = dfsPro(i,cur_time);
						cur_time += 1;
					}
				}
			}
			int end_time = cur_time;
			reversed_topological_list.push_back(u);
			return cur_time;
		}
		
		void bfs(int s)
		{
			cout<<"BFS"<<endl;
			vector<vector<int>> L;
			int n=total;
			L.resize(n);
			L[0].push_back(s);
			for(int i = 0; i<= n-1; i++)
			{
				cout<<"primer for "<<i<<endl;
				for(int u = 0; u < L[i].size(); u++)
				{
					cout<<"gg abc j size "<<adj[0].size()<<endl;
					for(int j = 0/*L[i][u]*/; j < adj[j].size() ;j++)
					{
						cout<<"gg abc 1"<<endl;
						//cout<<j<<" ";
						cout<<"l: "<<L[i][u]<<endl;
						if (adj[L[i][u]][j] == true)
						{
							cout<<"algun true"<<endl;
							
							if( visitado[j] == 0 )
							{
								cout<<"path: "<<endl;
								visitado[j] == 1;
								cout<<j<<" ";
								L[i+1].push_back(j);
								cout<<"push correcto"<<endl;
							}
							else
							   visitado[j] == 1;
						}
					}
				}
			}
		}
		
		void ciega(int src, int bus)
		{
			path2.clear();
			int c = 0;
			int p;
			bool *visited = new bool[total]; 
			for(int i = 0; i < total; i++) 
				visited[i] = false; 
			
			list<int> queue; 
			
			visited[src] = true; 
			queue.push_back(src); 
			
			
			list<int>::iterator i; 
			
			while(!queue.empty()) 
			{ 
				src = queue.front(); 
				cout << src << " "; 
				path2.push_back(points[src]);
				c = c+1;
				if (src == bus)
				{
					p = c;
					cout<<"FIN"<<endl;
				}
				queue.pop_front(); 
			
				for (i = adj11[src].begin(); i != adj11[src].end(); ++i) 
				{ 
					if (!visited[*i]) 
					{ 
						visited[*i] = true; 
						queue.push_back(*i); 
					} 
				} 
				
			} 
			path2.resize(p);
		} 
};

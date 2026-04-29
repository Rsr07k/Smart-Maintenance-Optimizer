#include "adjacency.h"
#include "road.h"
#include <iostream>
#include <iomanip>

using namespace std;


void buildAdjMatrix(int graph[100][100], int numNodes, int numEdges) {

    for (int i = 0; i < numNodes; i++)
        for (int j = 0; j < numNodes; j++)
            graph[i][j] = 0;

  
    for (int i = 0; i < numEdges; i++) {
        int u = Roads[i].u;
        int v = Roads[i].v;
        int cost = Roads[i].cost;

        graph[u][v] = cost;
        graph[v][u] = cost;
    }
}


void showMatrix(int graph[100][100], int numNodes) {

    cout << "\nAdjacency Matrix\n\n";

    cout << "    ";
    for (int j = 0; j < numNodes; j++)
        cout << setw(5) << j;
    cout << "\n";

    cout << "    ";
    for (int j = 0; j < numNodes; j++)
        cout << "-----";
    cout << "\n";

    for (int i = 0; i < numNodes; i++) {
        cout << setw(2) << i << " : ";
        for (int j = 0; j < numNodes; j++) {

            if (i == j || graph[i][j] == 0)
                cout << setw(5) << "-";
            else
                cout << setw(5) << graph[i][j];
        }
        cout << "\n";
    }
}
int arun(){
    int a = 0,b=1;
    return a+b;
}
int call(){
    int i = 5;
    while(i--){
        arun();
    }
    return 0;
}
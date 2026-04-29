#include <iostream>
#include "priority.h"
#include "graph.h"

using namespace std;

void setpriority(Road* roads, int E, int graph[100][100]) {
    for (int i = 0; i < E; i++) {
        roads[i].priority = roads[i].complaints * 5 + roads[i].damage * 3 + roads[i].traffic * 5;
        roads[i].priority += important(i, graph) * 5;
    }
}
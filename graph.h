#ifndef GRAPH_H
#define GRAPH_H

#include "road.h"
#include <vector>

int critical(const Road &road);

void criticalNodes(int graph[100][100], bool articulation[100]);
void findBridgesTarjan(std::vector<int>& bridgeIndices);

int important(int i,int graph[100][100]);

#endif

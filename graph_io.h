#ifndef GRAPH_IO_H
#define GRAPH_IO_H




#include <string>
#include <vector>


void inputGraph(int graph[100][100], int &budget, int &numEdges);


bool loadGraphFromFile(int graph[100][100], int &budget, int &numEdges,
                       const std::string &filename);

void saveToFile(int budget, int numEdges, const std::string &filename);



void saveResultsToFile(const std::vector<int> &dp_selected,
                       const std::vector<int> &greedy_selected);


void visualizeGraph(int budget, int numEdges);

#endif

#ifndef GRAPH_IO_H
#define GRAPH_IO_H

// graph_io.h
// Graph input/output operations


#include <string>
#include <vector>

// Enter graph manually
void inputGraph(int graph[100][100], int &budget, int &numEdges);

// Load graph from file
bool loadGraphFromFile(int graph[100][100], int &budget, int &numEdges,
                       const std::string &filename);

// Save graph to file
void saveToFile(int budget, int numEdges, const std::string &filename);


// Save results for JavaFX
void saveResultsToFile(const std::vector<int> &dp_selected,
                       const std::vector<int> &greedy_selected);

// Launch JavaFX visualizer
void visualizeGraph(int budget, int numEdges);

#endif

#ifndef ADJACENCY_H
#define ADJACENCY_H

// adjacency.h
// Handles building and displaying the adjacency matrix

// Fill graph[100][100] with costs from Roads[]
void buildAdjMatrix(int graph[100][100], int numNodes, int numEdges);

// Print the adjacency matrix
void showMatrix(int graph[100][100], int numNodes);

#endif

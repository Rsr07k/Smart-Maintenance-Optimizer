#ifndef ANALYSIS_H
#define ANALYSIS_H

// analysis.h
// Graph structural analysis

#include <vector>

// show articulation points
void showArticulationPoints(int graph[100][100], bool articulation[100],
                            bool &done);

// show bridge edges
void showBridges(std::vector<int> &bridgeIndices, bool &done);

#endif

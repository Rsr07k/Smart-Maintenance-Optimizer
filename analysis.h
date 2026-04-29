#ifndef ANALYSIS_H
#define ANALYSIS_H



#include <vector>


void showArticulationPoints(int graph[100][100], bool articulation[100],
                            bool &done);


void showBridges(std::vector<int> &bridgeIndices, bool &done);

#endif

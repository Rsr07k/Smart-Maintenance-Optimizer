#ifndef DP_H
#define DP_H


#include <vector>


std::vector<int> runDP(int budget, int numEdges,
                       std::vector<int> &bridgeIndices, bool &bridgesDone,
                       std::vector<int> &selected_bridges,
                       int &dp_total_cost, int &dp_total_priority);


void showDP(const std::vector<int> &dp_sel,
            const std::vector<int> &selected_bridges,
            int dp_total_cost, int dp_total_priority);

#endif

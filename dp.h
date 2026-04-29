#ifndef DP_H
#define DP_H

// ============================================================
// dp.h
// DP (Dynamic Programming) road optimization.
// Strategy:
//   1. Find bridge edges (critical roads that disconnect the graph)
//   2. Reserve budget for bridges first
//   3. Run 0/1 knapsack DP on remaining roads and budget
// DP road optimization wrapper

#include <vector>

// Run DP optimization.
// - Finds bridges if not already done (bridgesDone flag)
// - Fills selected_bridges with bridge road indices
// - Fills dp_total_cost and dp_total_priority
// - Returns indices of knapsack-selected roads (NOT including bridges)
std::vector<int> runDP(int budget, int numEdges,
                       std::vector<int> &bridgeIndices, bool &bridgesDone,
                       std::vector<int> &selected_bridges,
                       int &dp_total_cost, int &dp_total_priority);

// Print DP result: selected roads (bridges first, then knapsack),
// total cost, and total priority
void showDP(const std::vector<int> &dp_sel,
            const std::vector<int> &selected_bridges,
            int dp_total_cost, int dp_total_priority);

#endif

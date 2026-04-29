#include "dp.h"
#include "road.h"
#include "graph.h"
#include "knapsack.h"
#include <iostream>
#include <vector>

using namespace std;


vector<int> runDP(int budget, int numEdges,
                  vector<int> &bridgeIndices, bool &bridgesDone,
                  vector<int> &selected_bridges,
                  int &dp_total_cost, int &dp_total_priority) {

    if (!bridgesDone) {
        findBridgesTarjan(bridgeIndices);
        bridgesDone = true;
    }

    // sort bridges by priority/cost (simple bubble sort)
    for (int i = 0; i < (int)bridgeIndices.size() - 1; i++) {
        for (int j = 0; j < (int)bridgeIndices.size() - 1 - i; j++) {

            int a = bridgeIndices[j];
            int b = bridgeIndices[j + 1];

            double r1 = (Roads[a].cost == 0) ? 1e9 :
                        (double)Roads[a].priority / Roads[a].cost;

            double r2 = (Roads[b].cost == 0) ? 1e9 :
                        (double)Roads[b].priority / Roads[b].cost;

            if (r1 < r2) {
                int temp = bridgeIndices[j];
                bridgeIndices[j] = bridgeIndices[j + 1];
                bridgeIndices[j + 1] = temp;
            }
        }
    }

    
    for (int i = 0; i < numEdges; i++)
        Roads[i].rejected = false;

    selected_bridges.clear();

    int remaining = budget;
    int bridge_cost = 0;
    int bridge_priority = 0;

    // take bridges first
    for (int idx : bridgeIndices) {
        if (remaining >= Roads[idx].cost) {

            remaining -= Roads[idx].cost;
            bridge_cost += Roads[idx].cost;
            bridge_priority += Roads[idx].priority;

            selected_bridges.push_back(idx);
            Roads[idx].rejected = true;   // skip in knapsack
        }
    }

    
    vector<int> dp_selected = optimizeMaintenance(remaining, Roads, numEdges);

    // totals
    dp_total_cost = bridge_cost;
    dp_total_priority = bridge_priority;

    for (int idx : dp_selected) {
        dp_total_cost += Roads[idx].cost;
        dp_total_priority += Roads[idx].priority;
    }

    return dp_selected;
}


void showDP(const vector<int> &dp_sel,
            const vector<int> &selected_bridges,
            int dp_total_cost, int dp_total_priority) {

    cout << "\nSelected Roads:\n";

    if (selected_bridges.empty() && dp_sel.empty()) {
        cout << "None\n";
        return;
    }

    for (int idx : selected_bridges) {
        cout << Roads[idx].u << " - " << Roads[idx].v
             << "  (Cost: " << Roads[idx].cost
             << ", Priority: " << Roads[idx].priority << ")\n";
    }

    for (int idx : dp_sel) {
        cout << Roads[idx].u << " - " << Roads[idx].v
             << "  (Cost: " << Roads[idx].cost
             << ", Priority: " << Roads[idx].priority << ")\n";
    }

    cout << "\nTotal Cost: " << dp_total_cost << "\n";
    cout << "Total Priority: " << dp_total_priority << "\n";
}
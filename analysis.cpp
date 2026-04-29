#include "analysis.h"
#include "road.h"
#include "graph.h"
#include <iostream>
#include <vector>

using namespace std;


void showArticulationPoints(int graph[100][100], bool articulation[100],
                            bool &done) {

    if (!done) {
        criticalNodes(graph, articulation);
        done = true;
    }

    cout << "\nArticulation Points:\n";

    bool found = false;

    for (int i = 0; i < N; i++) {
        if (articulation[i]) {
            cout << "  Node " << i << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "  NONE\n";
}


void showBridges(vector<int> &bridgeIndices, bool &done) {

    if (!done) {
        findBridgesTarjan(bridgeIndices);
        done = true;
    }

    cout << "\nBridges:\n";

    if (bridgeIndices.empty()) {
        cout << "  NONE\n";
        return;
    }

    for (int idx : bridgeIndices) {
        cout << "  " << Roads[idx].u << " - " << Roads[idx].v
             << " (cost: " << Roads[idx].cost << ")\n";
    }
}
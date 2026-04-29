#include "greedy.h"
#include "road.h"
#include <iostream>
#include <vector>

using namespace std;


vector<int> runGreedy(int budget, int numEdges) {

    vector<int> idx;

    for (int i = 0; i < numEdges; i++)
        idx.push_back(i);

    // sort by priority/cost (simple sort)
    for (int i = 0; i < (int)idx.size() - 1; i++) {
        for (int j = 0; j < (int)idx.size() - 1 - i; j++) {

            int a = idx[j];
            int b = idx[j + 1];

            double r1 = (Roads[a].cost == 0) ? 1e9 :
                        (double)Roads[a].priority / Roads[a].cost;

            double r2 = (Roads[b].cost == 0) ? 1e9 :
                        (double)Roads[b].priority / Roads[b].cost;

            if (r1 < r2) {
                int temp = idx[j];
                idx[j] = idx[j + 1];
                idx[j + 1] = temp;
            }
        }
    }

    vector<int> selected;
    int remaining = budget;

    for (int i = 0; i < (int)idx.size(); i++) {

        int id = idx[i];

        if (Roads[id].cost <= remaining) {
            remaining -= Roads[id].cost;
            selected.push_back(id);
        }
    }

    return selected;
}

// show result
void showGreedy(const vector<int> &selected) {

    int totalCost = 0;
    int totalPriority = 0;

    for (int id : selected) {
        totalCost += Roads[id].cost;
        totalPriority += Roads[id].priority;
    }

    cout << "\nSelected Roads:\n";

    if (selected.empty()) {
        cout << "None\n";
        return;
    }

    for (int id : selected) {
        cout << Roads[id].u << " - " << Roads[id].v
             << "  (Cost: " << Roads[id].cost
             << ", Priority: " << Roads[id].priority << ")\n";
    }

    cout << "\nTotal Cost: " << totalCost << "\n";
    cout << "Total Priority: " << totalPriority << "\n";
}
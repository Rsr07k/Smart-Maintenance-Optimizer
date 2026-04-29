#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <chrono>
#include <string>

#include "road.h"
#include "priority.h"
#include "adjacency.h"
#include "graph_io.h"
#include "greedy.h"
#include "dp.h"
#include "analysis.h"

using namespace std;


int rahul(){
        return 0;
    }

int main() {

    int budget = 0;
    int numEdges = 0;
    int graph[100][100];

    bool graphInitialized = false;
    bool prioritiesSet = false;
    bool optimizationRun = false;
    bool articulationDone = false;
    bool bridgesDone = false;

    vector<int> dp_selected;
    vector<int> selected_bridges;
    vector<int> greedy_selected;
    vector<int> bridgeIndices;

    bool articulation[100] = {false};

    int dp_total_cost = 0;
    int dp_total_priority = 0;

    

    while (true) {

        cout << "\nROAD OPTIMIZATION SYSTEM\n";
        cout << "1. Enter Graph\n";
        cout << "2. Load Graph from File\n";
        cout << "3. Show Adjacency Matrix\n";
        cout << "4. Show Roads with Priority\n";
        cout << "5. Run DP Optimization\n";
        cout << "6. Run Greedy Optimization\n";
        cout << "7. Show Articulation Points & Bridges\n";
        cout << "8. Save Graph to File\n";
        cout << "9. Visualize Graph (JavaFX)\n";
        cout << "10. Detailed Visualization (Web Page)\n";
        cout << "11. Exit\n";
        cout << "Choose option: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {

            case 1: {
                inputGraph(graph, budget, numEdges);

                graphInitialized = true;
                prioritiesSet = false;
                optimizationRun = false;
                articulationDone = false;
                bridgesDone = false;

                bridgeIndices.clear();
                dp_selected.clear();
                greedy_selected.clear();
                selected_bridges.clear();
                break;
            }

            case 2: {
                if (loadGraphFromFile(graph, budget, numEdges, "input.txt")) {
                    graphInitialized = true;
                    prioritiesSet = false;
                    optimizationRun = false;
                    articulationDone = false;
                    bridgesDone = false;

                    bridgeIndices.clear();
                    dp_selected.clear();
                    greedy_selected.clear();
                    selected_bridges.clear();
                }
                break;
            }

            case 3: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }
                showMatrix(graph, N);
                break;
            }

            case 4: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }

                if (!prioritiesSet) {
                    setpriority(Roads, numEdges, graph);
                    for (int i = 0; i < numEdges; i++)
                        if (Roads[i].priority == 0) Roads[i].priority = 5;
                    prioritiesSet = true;
                }

                cout << "\nRoads with Priority\n";
                cout << setw(5) << "ID" << setw(5) << "U" << setw(5) << "V"
                     << setw(8) << "Cost" << setw(10) << "Priority\n";
                cout << "---------------------------------\n";

                for (int i = 0; i < numEdges; i++) {
                    cout << setw(5) << Roads[i].id
                         << setw(5) << Roads[i].u
                         << setw(5) << Roads[i].v
                         << setw(8) << Roads[i].cost
                         << setw(10) << Roads[i].priority << "\n";
                }
                break;
            }

            case 5: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }

                if (!prioritiesSet) {
                    setpriority(Roads, numEdges, graph);
                    for (int i = 0; i < numEdges; i++)
                        if (Roads[i].priority == 0) Roads[i].priority = 5;
                    prioritiesSet = true;
                }

                dp_selected = runDP(budget, numEdges,
                                    bridgeIndices, bridgesDone,
                                    selected_bridges,
                                    dp_total_cost, dp_total_priority);

                showDP(dp_selected, selected_bridges, dp_total_cost, dp_total_priority);

                optimizationRun = true;

                vector<int> all_dp = selected_bridges;
                for (int x : dp_selected) all_dp.push_back(x);

                saveResultsToFile(all_dp, greedy_selected);
                break;
            }

            case 6: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }

                if (!prioritiesSet) {
                    setpriority(Roads, numEdges, graph);
                    for (int i = 0; i < numEdges; i++)
                        if (Roads[i].priority == 0) Roads[i].priority = 5;
                    prioritiesSet = true;
                }

                greedy_selected = runGreedy(budget, numEdges);
                showGreedy(greedy_selected);

                if (optimizationRun) {
                    int g = 0;
                    for (int x : greedy_selected)
                        g += Roads[x].priority;

                    cout << "\nDP: " << dp_total_priority;
                    cout << "\nGreedy: " << g << "\n";

                    if (dp_total_priority >= g)
                        cout << "Better: DP\n";
                    else
                        cout << "Better: Greedy\n";
                } else {
                    cout << "\nRun DP first for comparison\n";
                }

                vector<int> all_dp = selected_bridges;
                for (int x : dp_selected) all_dp.push_back(x);
                saveResultsToFile(all_dp, greedy_selected);

                break;
            }

            case 7: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }
                showArticulationPoints(graph, articulation, articulationDone);
                showBridges(bridgeIndices, bridgesDone);
                break;
            }

            case 8: {
                if (!graphInitialized) {
                    cout << "Load graph first.\n";
                    break;
                }
                saveToFile(budget, numEdges, "input.txt");
                break;
            }

            case 9: {
                visualizeGraph(budget, numEdges);
                break;
            }

            case 10: {
                cout << "Opening web view...\n";
                system("start visualization.html");
                break;
            }

            case 11:
                cout << "Exit\n";
                return 0;

            default:
                cout << "Invalid option\n";
        }
    }
    int x = rahul();
}
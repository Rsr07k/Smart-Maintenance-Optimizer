#include "graph_io.h"
#include "adjacency.h"
#include "road.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>

using namespace std;

// take input from user
void inputGraph(int graph[100][100], int &budget, int &numEdges) {

    cout << "Budget: ";
    cin >> budget;

    cout << "Nodes (N): ";
    cin >> N;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < N; i++) {
        cout << "Node " << i << " type (Important / Not-important): ";
        string type;
        getline(cin, type);
        if (type.empty()) type = "Not-important";
        nodes[i] = Node(type, i);
    }

    cout << "Roads (E): ";
    cin >> numEdges;
    E = numEdges;

    for (int i = 0; i < numEdges; i++) {

        cout << "\nRoad " << i << "\n";

        int u, v, cost, traffic, damage, complaints;

        do {
            cout << "Start node: ";
            cin >> u;
        } while (u < 0 || u >= N);

        do {
            cout << "End node: ";
            cin >> v;
        } while (v < 0 || v >= N);

        cout << "Cost: ";
        cin >> cost;

        cout << "Traffic (0-5): ";
        cin >> traffic;

        cout << "Damage (0-5): ";
        cin >> damage;

        cout << "Complaints: ";
        cin >> complaints;

        Roads[i] = Road(i, u, v, cost, traffic, damage, complaints);
        Roads[i].rejected = false;
    }

    buildAdjMatrix(graph, N, numEdges);

    cout << "\nGraph loaded:\n";
    for (int i = 0; i < numEdges; i++) {
        cout << Roads[i].u << " --" << Roads[i].cost << "-- " << Roads[i].v << "\n";
    }
}

// load from file
bool loadGraphFromFile(int graph[100][100], int &budget, int &numEdges,
                       const string &filename) {

    ifstream fin(filename);
    if (!fin) {
        cout << "File not found.\n";
        return false;
    }

    fin >> budget;
    fin >> N;
    fin.ignore();

    for (int i = 0; i < N; i++) {
        string type;
        getline(fin, type);
        if (type.empty()) type = "Not-important";
        nodes[i] = Node(type, i);
    }

    fin >> numEdges;
    E = numEdges;

    for (int i = 0; i < numEdges; i++) {
        int u, v, cost, traffic, damage, complaints;
        fin >> u >> v >> cost >> traffic >> damage >> complaints;
        Roads[i] = Road(i, u, v, cost, traffic, damage, complaints);
        Roads[i].rejected = false;
    }

    fin.close();

    buildAdjMatrix(graph, N, numEdges);

    cout << "\nGraph loaded from file\n";
    for (int i = 0; i < numEdges; i++) {
        cout << Roads[i].u << " --" << Roads[i].cost << "-- " << Roads[i].v << "\n";
    }

    return true;
}

// save graph
void saveToFile(int budget, int numEdges, const string &filename) {

    ofstream out(filename);

    out << budget << "\n";
    out << N << "\n";

    for (int i = 0; i < N; i++)
        out << nodes[i].type << "\n";

    out << numEdges << "\n";

    for (int i = 0; i < numEdges; i++) {
        out << Roads[i].u << " "
            << Roads[i].v << " "
            << Roads[i].cost << " "
            << Roads[i].traffic << " "
            << Roads[i].damage << " "
            << Roads[i].complaints << "\n";
    }

    out.close();
    cout << "Saved to " << filename << "\n";
}

// save results
void saveResultsToFile(const vector<int> &dp_selected,
                       const vector<int> &greedy_selected) {

    ofstream out("results.txt");

    out << "DP\n";
    out << dp_selected.size() << "\n";

    for (int idx : dp_selected)
        out << Roads[idx].u << " " << Roads[idx].v << "\n";

    out << "GREEDY\n";
    out << greedy_selected.size() << "\n";

    for (int idx : greedy_selected)
        out << Roads[idx].u << " " << Roads[idx].v << "\n";

    out.close();
}

// open JavaFX
void visualizeGraph(int budget, int numEdges) {

    saveToFile(budget, numEdges, "input.txt");

    cout << "Opening graph window...\n";

    string javafxLib =
        "C:/Users/hp/Downloads/openjfx-21.0.10_windows-x64_bin-sdk"
        "/javafx-sdk-21.0.10/lib";

    string compileCmd =
        "javac --module-path \"" + javafxLib + "\""
        " --add-modules javafx.controls,javafx.fxml"
        " GraphVisualizer.java";

    if (system(compileCmd.c_str()) != 0) {
        cout << "Compilation failed.\n";
        return;
    }

    string runCmd =
        "start java --module-path \"" + javafxLib + "\""
        " --add-modules javafx.controls,javafx.fxml"
        " GraphVisualizer";

    system(runCmd.c_str());
}
#include "graph.h"

#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

int critical(const Road &road) {

    int u = road.getu();
    int v = road.getv();

    vector<int> adj[100];

    for (int i = 0; i < E; i++) {
        if (Roads[i].id == road.id) continue;

        int a = Roads[i].getu();
        int b = Roads[i].getv();

        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    bool visited[100] = {false};

    vector<int> st;
    st.push_back(u);
    visited[u] = true;

    while (!st.empty()) {

        int x = st.back();
        st.pop_back();

        for (int nb : adj[x]) {
            if (!visited[nb]) {
                visited[nb] = true;
                st.push_back(nb);
            }
        }
    }

    return visited[v] ? 0 : 1;
}

// count how many important nodes connected
int important(int i, int graph[100][100]) {

    int imp = 0;

    if (nodes[Roads[i].getu()].type == "Important") imp++;
    if (nodes[Roads[i].getv()].type == "Important") imp++;

    return imp;
}

// dfs for articulation points
void apDfs(int u, int graph[100][100], bool visited[100],
           int disc[100], int low[100], int parent[100],
           bool articulation[100], int &time) {

    visited[u] = true;
    disc[u] = low[u] = ++time;

    int children = 0;

    for (int v = 0; v < N; v++) {

        if (graph[u][v] == 0) continue;

        if (!visited[v]) {

            children++;
            parent[v] = u;

            apDfs(v, graph, visited, disc, low, parent, articulation, time);

            low[u] = min(low[u], low[v]);

            if (parent[u] == -1 && children > 1)
                articulation[u] = true;

            if (parent[u] != -1 && low[v] >= disc[u])
                articulation[u] = true;
        }
        else if (v != parent[u]) {
            low[u] = min(low[u], disc[v]);
        }
    }
}

// find articulation points
void criticalNodes(int graph[100][100], bool articulation[100]) {

    int disc[100], low[100], parent[100];
    bool visited[100];

    for (int i = 0; i < N; i++) {
        disc[i] = 0;
        low[i] = 0;
        parent[i] = -1;
        visited[i] = false;
        articulation[i] = false;
    }

    int time = 0;

    for (int i = 0; i < N; i++) {
        if (!visited[i])
            apDfs(i, graph, visited, disc, low, parent, articulation, time);
    }
}

// dfs for bridges
void bridgeDfs(int u, int parent_node, vector<int> adj[],
               int disc[], int low[], bool visited[],
               vector<int>& bridges, int &time) {

    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (int edgeIdx : adj[u]) {

        int v = (Roads[edgeIdx].getu() == u)
                ? Roads[edgeIdx].getv()
                : Roads[edgeIdx].getu();

        if (!visited[v]) {

            bridgeDfs(v, u, adj, disc, low, visited, bridges, time);

            low[u] = min(low[u], low[v]);

            if (low[v] > disc[u])
                bridges.push_back(edgeIdx);
        }
        else if (v != parent_node) {
            low[u] = min(low[u], disc[v]);
        }
    }
}

// find bridges
void findBridgesTarjan(vector<int>& bridgeIndices) {

    vector<int> adj[100];

    for (int i = 0; i < E; i++) {
        int u = Roads[i].getu();
        int v = Roads[i].getv();

        adj[u].push_back(i);
        adj[v].push_back(i);
    }

    int disc[100], low[100];
    bool visited[100];

    for (int i = 0; i < N; i++) {
        visited[i] = false;
        disc[i] = 0;
        low[i] = 0;
    }

    int time = 0;

    for (int i = 0; i < N; i++) {
        if (!visited[i])
            bridgeDfs(i, -1, adj, disc, low, visited, bridgeIndices, time);
    }
}
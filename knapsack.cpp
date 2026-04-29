#include "knapsack.h"
#include <vector>
#include <algorithm>

using namespace std;

vector<int> optimizeMaintenance(int budget, Road roads[], int E) {

    vector<int> valid;

    // take only non-rejected roads
    for (int i = 0; i < E; i++) {
        if (!roads[i].rejected)
            valid.push_back(i);
    }

    int n = valid.size();
    if (n == 0) return {};

    vector<vector<int>> dp(n + 1, vector<int>(budget + 1, 0));

    // build dp table
    for (int i = 1; i <= n; i++) {

        int id = valid[i - 1];
        int cost = roads[id].cost;
        int val  = roads[id].priority;

        for (int w = 0; w <= budget; w++) {

            if (cost <= w) {
                dp[i][w] = max(dp[i - 1][w],
                               dp[i - 1][w - cost] + val);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // backtrack to find selected roads
    vector<int> selected;

    int res = dp[n][budget];
    int w = budget;

    for (int i = n; i > 0 && res > 0; i--) {

        if (res == dp[i - 1][w]) {
            continue;
        } else {
            int id = valid[i - 1];

            selected.push_back(id);

            res -= roads[id].priority;
            w   -= roads[id].cost;
        }
    }

    reverse(selected.begin(), selected.end());
    
    return selected;
}

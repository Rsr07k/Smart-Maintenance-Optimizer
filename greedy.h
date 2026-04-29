#ifndef GREEDY_H
#define GREEDY_H

// greedy.h
// Greedy road selection algorithm

#include <vector>

// Run greedy algorithm
std::vector<int> runGreedy(int budget, int numEdges);

// Output greedy result
void showGreedy(const std::vector<int> &selected);

#endif

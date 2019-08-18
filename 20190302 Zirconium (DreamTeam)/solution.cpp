#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <utility>

using namespace std;

int solution(vector<int>& A, vector<int>& B, int F) {
    int N = A.size(), E = N - F; // E = number of backend devs

    // The idea: calculate the "frontend benefit" for each developer; that is, the relative
    // difference to the team strength when they are chosen as frontend vs as backend.
    // Keep a reference to the dev index number with each benefit so we know who provides it.
    vector<pair<int, int>> frontend_benefit(N);

    for (auto i = 0; i < N; ++i)
    {
        frontend_benefit[i] = make_pair(i, A[i] - B[i]);
    }

    // Partial sort the first E items, i.e. developers with lowest frontend benefit;
    // this is the same as having the highest backend benefit.
    partial_sort(
        frontend_benefit.begin(), 
        frontend_benefit.begin() + E + 1, 
        frontend_benefit.end(),
        [](auto p1, auto p2) { return p1.second < p2.second; }
    );

    // Now simply calculate total strength
    int total_strength = 0;
    for (auto i = 0; i < E; ++i)
    {
        total_strength += B[frontend_benefit[i].first];
    }

    for (auto i = E; i < N; ++i)
    {
        total_strength += A[frontend_benefit[i].first];
    }

    return total_strength;
}

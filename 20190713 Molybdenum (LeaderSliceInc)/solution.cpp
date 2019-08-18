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

// K = segment length
// M = upper bound of items in A
// N and M are integers within the range[1..100, 000];
// K is an integer within the range[1..N];
// each element of array A is an integer within the range[1..M].
vector<int> solution(int K, int M, vector<int>&& A)
{
    set<int> leaders;
    map<int, int> global_occurrences, segment_occurrences;

    int size = A.size(), threshold = size / 2; // if you can't pass the threshold, you can't become a leader
    auto begin = A.begin(), end = A.end();

    for (auto i = begin; i != end; ++i)
    {
        global_occurrences[*i]++;
    }

    int natural_leader = 0; // at most one number can be a leader by considering a segment wherein it does NOT appear AFTER it has been incremented
    for (auto t : global_occurrences)
    {
        if (t.second > threshold)
        {
            natural_leader = t.first;
            break;
        }
    }

    // note 1 less window size than K, the final element is added on loop start
    for (auto i = begin, stop = begin + K - 1; i != stop; ++i)
    {
        segment_occurrences[*i]++;
    }

    for (auto i = 0; (size_t) (i + K - 1) < size; ++i)
    {
        segment_occurrences[A[i + K - 1]]++;

        for (auto t : segment_occurrences)
        {
            int candidate = t.first + 1;
            int segment_population = t.second;
            int lost_population = segment_occurrences.count(candidate) ? segment_occurrences[candidate] : 0;
            int global_population = global_occurrences.count(candidate) ? global_occurrences[candidate] : 0;
            int candidate_occurrences = global_population + segment_population - lost_population;

            if (candidate_occurrences > threshold)
            {
                leaders.insert(candidate);
            }
        }

        int natural_leader_lost_population = segment_occurrences.count(natural_leader) ? segment_occurrences[natural_leader] : 0;
        if (global_occurrences[natural_leader] - natural_leader_lost_population > threshold)
        {
            leaders.insert(natural_leader);
        }

        if (--segment_occurrences[A[i]] == 0)
        {
            segment_occurrences.erase(A[i]);
        }
    }

    return vector<int>(leaders.begin(), leaders.end());
}

int main()
{
    vector<int> result;
    //auto result = solution(3, 5, { 2, 1, 3, 1, 2, 2, 3 });

    //result = solution(4, 2, { 1, 2, 2, 1, 2 });
    //print_vector(result, "solution");

    result = solution(2, 5, { 1, 1, 1, 1, 1 });
    //print_vector(result, "solution");

    result = solution(2, 5, { 2, 2, 1, 1, 1 });
    //print_vector(result, "solution");

    for (auto i : result) cout << i; cout << endl;

    result = solution(2, 5, { 1, 3, 1, 4, 1, 1 });
    //print_vector(result, "solution");

    for (auto i : result) cout << i; cout << endl;
    
    return 0;
}



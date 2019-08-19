#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

struct run_properties_t
{
    char letter = 0;
    int length = 0;
};

int solution(vector<string>& words)
{
    set<char> interesting_letters;
    map<char, int> sum_of_uniform_word_lengths;
    map<char, multiset<int>> two_longest_prefixes;
    map<char, multiset<int>> two_longest_suffixes;
    map<char, int> disqualified_prefix_length;
    map<char, int> disqualified_suffix_length;

    int longest_run_length = 0; // this will be updated until it becomes the final result

    // Stage 1: iterate over the input and collect stats
    for (auto word : words)
    {
        int word_length = word.size();
        run_properties_t prefix, suffix, longest;
        prefix.letter = word.front();
        suffix.letter = word.back();

        int run_start_index = 0;
        char run_letter = prefix.letter;

        for (auto i = 0; i < word_length; ++i)
        {
            if (word[i] != run_letter)
            {
                if (run_start_index == 0)
                {
                    prefix.length = i;
                }
                if (longest.length < i - run_start_index) // not elseif, as prefix can be also longest run
                {
                    longest.letter = run_letter;
                    longest.length = i - run_start_index;
                }

                run_start_index = i;
                run_letter = word[i];
            }
        }

        suffix.length = word_length - run_start_index;

        if (prefix.length == 0)
        {
            // We never encountered a letter that finalizes the prefix, so word consists entirely of one letter
            prefix.length = word_length;
        }

        if (longest.length == 0)
        {
            // We never encountered a letter that finalizes the longest run, so that is also the suffix;
            // note this does not only set the length for max correctness, although we could get away with it
            longest = suffix;
        }

        if (longest_run_length < longest.length)
        {
            longest_run_length = longest.length;
        }

        if (longest.length == word_length)
        {
            sum_of_uniform_word_lengths[prefix.letter] += word_length;
        }
        else
        {
            if (!two_longest_prefixes[prefix.letter].size())
            {
                two_longest_prefixes[prefix.letter].insert({ 0, 0 });
            }
            if (!two_longest_suffixes[suffix.letter].size())
            {
                two_longest_suffixes[suffix.letter].insert({ 0, 0 });
            }

            // How to easily keep "two highest" numbers: insert candidate into an ordered multiset
            // of size exactly two, then remove the first (smallest) item
            two_longest_prefixes[prefix.letter].insert(prefix.length);
            two_longest_prefixes[prefix.letter].erase(two_longest_prefixes[prefix.letter].begin());
            two_longest_suffixes[suffix.letter].insert(suffix.length);
            two_longest_suffixes[suffix.letter].erase(two_longest_suffixes[suffix.letter].begin());

            // If this word has affixes consisting of the same letter, if any of the two are the
            // longest ever seen of its type, disqualify the other when considering concatenation
            if (prefix.letter == suffix.letter)
            {
                if (prefix.length == *two_longest_prefixes[prefix.letter].rbegin())
                {
                    disqualified_suffix_length[suffix.letter] = suffix.length;
                }
                if (suffix.length == *two_longest_suffixes[suffix.letter].rbegin())
                {
                    disqualified_prefix_length[prefix.letter] = prefix.length;
                }
            }           
        }
        
        interesting_letters.insert({ prefix.letter, suffix.letter, longest.letter });
    }

    // Stage 2: calculate the longest possible run produced by concatenating words
    for (auto ch : interesting_letters)
    {
        // Maybe all the words consisting entirely of a single letter concatenated together?
        longest_run_length = max(
            longest_run_length, 
            sum_of_uniform_word_lengths[ch]
        );

        // Maybe we can do better by concatenating the longest prefix with longest non-disqualified suffix?
        if (two_longest_prefixes.count(ch)) {
            auto longest_prefix = *two_longest_prefixes[ch].rbegin();
            auto longest_non_disqualified_suffix = 0;

            if (two_longest_suffixes.count(ch)) {
                auto suffix_length_iterator = two_longest_suffixes[ch].rbegin();
                if (disqualified_suffix_length[ch] == *suffix_length_iterator) ++suffix_length_iterator;
                longest_non_disqualified_suffix = *suffix_length_iterator;
            }

            longest_run_length = max(
                longest_run_length,
                longest_prefix + sum_of_uniform_word_lengths[ch] + longest_non_disqualified_suffix
            );
        }

        // Maybe we can do better by concatenating the longest suffix with longest non-disqualified prefix?
        if (two_longest_suffixes.count(ch)) {
            auto longest_suffix = *two_longest_suffixes[ch].rbegin();
            auto longest_non_disqualified_prefix = 0;

            if (two_longest_prefixes.count(ch)) {
                auto prefix_length_iterator = two_longest_prefixes[ch].rbegin();
                if (disqualified_prefix_length[ch] == *prefix_length_iterator) ++prefix_length_iterator;
                longest_non_disqualified_prefix = *prefix_length_iterator;
            }

            longest_run_length = max(
                longest_run_length,
                longest_non_disqualified_prefix + sum_of_uniform_word_lengths[ch] + longest_suffix
            );
        }
    }

    return longest_run_length;
}

int main()
{
    vector<string> input;

    //input = { "aabb", "aaaa", "bbab" };
    //cout << "result = " << solution(input) << endl << endl; // 6


    //input = { "xxbxx", "xbx", "x" };
    //cout << "result = " << solution(input) << endl << endl; // 4


    //input = { "dd", "bb", "cc", "dd" };
    //cout << "result = " << solution(input) << endl << endl; // 4

    //input = { "aaaxxxxxxxa", "bb", "b", "ab", "bba" };
    //cout << "result = " << solution(input) << endl << endl; // 7

    //input = { "bb", "b", "aaa", "ab", "bba" };
    //cout << "result = " << solution(input) << endl << endl; // 6

    input = { "abaa", "aba", "cc", "dd" };
    cout << "result = " << solution(input) << endl << endl; // 3
    return 0;
}


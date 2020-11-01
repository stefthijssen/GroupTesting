#include <iostream>
#include <vector>
#include <algorithm>

#include "../headers/input.hpp"
#include "../headers/global.hpp"

#include "../utils/testHelpers.cpp"
#include "../utils/formulas.cpp"

#include "./adjacencyMatrix.cpp"

using namespace std;

vi customGroupTestSplit(vi nodes, vector<bool> &infected, Input input)
{
    int size = nodes.size();

    cout << "test";
    for (int i = 0; i < size; i++)
    {
        cout << " " << nodes.at(i);
    }

    cout << endl
         << flush << endl;
    nTests = nTests + 1;

    bool result = retrieveTestResult();

    if (result == false)
    {
        for (int i = 0; i < size; i++)
        {
            nonInfectedFound++;
        }
    }

    if (result == true)
    {
        return nodes;
    }
    else
    {
        vi empty;
        return empty;
    }
}

void poolTest(vi nodes, vector<bool> &infected, Input input)
{
    int currentIndex = 0;

    while (currentIndex <= nodes.size())
    {
        int k = calculateK(input);

        int start = currentIndex;
        int end = currentIndex + k;

        if (end > nodes.size())
        {
            end = nodes.size();
        }

        if (currentIndex - end == 1)
        {
            testIndividual(nodes.at(currentIndex));
            break;
        }

        if (remainingTestsAreNegative(input))
        {
            for (size_t i = currentIndex; i < nodes.size(); i++)
            {
                infected[nodes.at(i)] = false;
                nonInfectedFound++;
            }
            break;
        }

        if (remainingTestsArePositive(input))
        {
            for (size_t i = currentIndex; i < nodes.size(); i++)
            {
                infected[nodes.at(i)] = true;
                infectedFound++;
            }
            break;
        }

        vector<int>::const_iterator first = nodes.begin() + start;
        vector<int>::const_iterator last = nodes.begin() + end;
        vector<int> subgroup(first, last);

        bool isGroupInfected = testPooledSamples(subgroup);
        if (isGroupInfected == true)
        {
            testOneByOneEfficient(subgroup, infected, input, true);
        }
        currentIndex = currentIndex + k;
    }
}

void usePoolTest(Input input, AdjacencyMatrix adjMatrix, vector<bool> &infected)
{
    vi nodes;

    vector<vector<int>> pairs;

    pairs = adjMatrix.findDenseSubGraph(0, pairs, 0);
    if (input.infectionChance >= 0.6) // High infection chance, lets first check less likely nodes, with less edges and less matching edges. Pair them in groups to eliminate slightly faster.
    {
        std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() < b.size(); });
    }
    else
    {
        std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() > b.size(); });
    }

    for (size_t i = 0; i < pairs.size(); i++)
    {
        for (size_t j = 0; j < pairs.at(i).size(); j++)
        {
            nodes.push_back(pairs.at(i).at(j));
        }
    }

    poolTest(nodes, infected, input);
}
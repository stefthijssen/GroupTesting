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

    if (result == true && size == 1)
    {
        infectedFound++;
    }
    
    for (int i = 0; i < size; i++)
    {
        infected[nodes.at(i)] = result;
        if (result == false)
        {
            nonInfectedFound++;
        }
    }

    if (result == true && size != 1)
    {
        return nodes;
    }
    else
    {
        vi empty;
        return empty;
    }
}

void poolTest(int groupSize, vi nodes, vector<bool> &infected, Input input)
{
    int size = groupSize;
    int currentIndex = 0;
    vvi pairs;

    while (currentIndex <= nodes.size())
    {
        int end = currentIndex + size;
        if (currentIndex + size > nodes.size())
        {
            end = nodes.size();
        }

        vector<int>::const_iterator first = nodes.begin() + currentIndex;
        vector<int>::const_iterator last = nodes.begin() + end;
        vector<int> subgroup(first, last);
        vi result = customGroupTestSplit(subgroup, infected, input);
        currentIndex = currentIndex + size;

        if (result.size() > 1)
        {
            bool isDone = oneByOneTest(result, infected, input);
            size = calculateK(input);
            if(isDone == true){
                return;
            }
        }
        
    }
}

void usePoolTest(Input input, AdjacencyMatrix adjMatrix, vector<bool> &infected) {
    vi nodes;

    vector<vector<int>> pairs;

    pairs = adjMatrix.findDenseSubGraph(0, pairs, 0);
    std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() > b.size(); });

    for (size_t i = 0; i < pairs.size(); i++)
    {
        for (size_t j = 0; j < pairs.at(i).size(); j++)
        {
            nodes.push_back(pairs.at(i).at(j));
        }
    }

    int k = calculateK(input);
    poolTest(k, nodes, infected, input);
}
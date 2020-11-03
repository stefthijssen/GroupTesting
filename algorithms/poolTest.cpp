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
    float p = calculateP(input);
    int k = calculateK(input, p);

    while (currentIndex < nodes.size())
    {
        if ((input.maxInfected - input.minInfected) == 0)
        {
            p = calculateP(input);
            k = calculateK(input, p);
            if (p <= 0)
            {
                break;
            }
        }
        int start = currentIndex;
        int end = currentIndex + k;

        if (end > nodes.size())
        {
            end = nodes.size();
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

        if (currentIndex - end == 1)
        {
            testIndividual(nodes.at(currentIndex));
            currentIndex = end;
            continue;
        }

        vector<int>::const_iterator first = nodes.begin() + start;
        vector<int>::const_iterator last = nodes.begin() + end;
        vector<int> subgroup(first, last);

        bool isGroupInfected = testPooledSamples(subgroup);

        if (isGroupInfected == true)
        {
            if (k >= 4)
            {
                int middle = (start + end) / 2;
                vector<int>::const_iterator lfirst = nodes.begin() + start;
                vector<int>::const_iterator llast = nodes.begin() + middle;
                vector<int> left(lfirst, llast);
                vector<int>::const_iterator rfirst = nodes.begin() + middle;
                vector<int>::const_iterator rlast = nodes.begin() + end;
                vector<int> right(rfirst, rlast);
                bool leftInfected = testPooledSamples(left);
                bool rightInfected = testPooledSamples(right);
                if (leftInfected != true && rightInfected == true)
                {
                    subgroup = right;
                }
                if (leftInfected == true && rightInfected != true)
                {
                    subgroup = left;
                }
            }
            testOneByOneEfficient(subgroup, infected, input, true);
        }
        currentIndex = end;
    }
}

void usePoolTest(Input input, AdjacencyMatrix adjMatrix, vector<bool> &infected)
{
    vi nodes;

    vector<vector<int>> pairs;
    adjMatrix.calculateAffinity();
    
    pairs = adjMatrix.findDenseSubGraph(0, pairs);

    for (size_t i = 0; i < pairs.size(); i++)
    {
        for (size_t j = 0; j < pairs.at(i).size(); j++)
        {
            nodes.push_back(pairs.at(i).at(j));
        }
    }

    poolTest(nodes, infected, input);
}
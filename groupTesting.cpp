#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <tgmath.h>

#include "utils/credentials.cpp"
#include "utils/testHelpers.cpp"
#include "utils/inputParser.cpp"

#include "algorithms/adjacencyMatrix.cpp"
#include "algorithms/splitTest.cpp"

#include "headers/input.h"
#include "headers/global.h"

using namespace std;

int recursionThreshold = 10;
float thresholdRecursionStep = 0.1;

int calculateK(Input input)
{
    //default to k = 8
    int k = 8;
    float nAverageUpperboundInfected = (float)input.maxInfected;
    float nAverageLowerboundInfected = (float)input.minInfected;
    float averageInfectionRate = (float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / (input.nNodes * 2);

    // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
    float p = (float)averageInfectionRate + ((float)averageInfectionRate * (float)input.infectionChance);
    if (p >= 0.15)
    {
        k = 3;
    }
    if (p >= 0.1)
    {
        k = 4;
    }
    if (p >= 0.05)
    {
        k = 7;
    }
    if (p >= 0.01)
    {
        k = 11;
    }
    return k;
}

void recursiveTest(AdjacencyMatrix &adjMatrix, vvi inputPairs, vector<bool> &infected, float threshold, Input input)
{
    unvisitPairsInMatrix(inputPairs, adjMatrix);
    vector<vector<int>> pairs;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, threshold);

    std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() > b.size(); });
    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testNodes(pairs.at(i));
        updateInfected(pairs.at(i), infected, toTest);
    }

    if (toTest.size() > 0)
    {
        vvi recursiveTestPairs;
        vvi manualTestPairs;

        for (int i = 0; i < toTest.size(); i++)
        {
            if (toTest.at(i).size() > recursionThreshold)
            {
                recursiveTestPairs.push_back(toTest.at(i));
            }
            else
            {
                manualTestPairs.push_back(toTest.at(i));
            }
        }

        float newThreshold = threshold - thresholdRecursionStep;
        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold, input);
        manualTest(manualTestPairs, infected, input);
    }
}

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
    }

    if (result == true && size != 1)
    {
        // cerr << "size is " << size << endl;
        return nodes;
    }
    else
    {
        vi empty;
        return empty;
    }
}

void runTestCaseGrouped(int groupSize, vi nodes, vector<bool> &infected, Input input)
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
        // cerr << "begin " << currentIndex << " end " << end;
        currentIndex = currentIndex + size;

        if (result.size() > 1)
        {
            pairs.push_back(result);
        }

        // if (remainingTestsAreNegative(input))
        // {
        //     return;
        // }
    }
    if (groupSize >= 8 && pairs.size() > 2)
    {

        vi newNodes;
        for (size_t i = 0; i < pairs.size(); i++)
        {
            for (size_t j = 0; j < pairs.at(i).size(); j++)
            {
                newNodes.push_back(pairs.at(i).at(j));
            }
        }
        groupSize = groupSize / 2;
        runTestCaseGrouped(groupSize, newNodes, infected, input);
    }
    else
    {
        manualTest(pairs, infected, input);
    }
}

void runTestCaseSpreadGraph(vi nodes, vector<bool> &infected, Input input)
{
    int middle = nodes.size() / 2;
    int infectedCounter = 0;

    splitTest(nodes, 0, middle, infected, input);
    splitTest(nodes, middle, nodes.size(), infected, input);
}

void runTestCaseGroupedGraph(AdjacencyMatrix &adjMatrix, vector<bool> &infected, Input input)
{
    vector<vector<int>> pairs;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, baseThreshold);
    std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() > b.size(); });
    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testNodes(pairs.at(i));
        updateInfected(pairs.at(i), infected, toTest);
        if (remainingTestsAreNegative(input))
        {
            return;
        }
    }

    if (toTest.size() > 0)
    {
        vvi recursiveTestPairs;
        vvi manualTestPairs;

        for (int i = 0; i < toTest.size(); i++)
        {
            if (toTest.at(i).size() > recursionThreshold)
            {
                recursiveTestPairs.push_back(toTest.at(i));
            }
            else
            {
                manualTestPairs.push_back(toTest.at(i));
            }
        }

        float newThreshold = baseThreshold - thresholdRecursionStep;

        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold, input);
        manualTest(manualTestPairs, infected, input);
    }
}


void runTestCases(int numCase, int &numCorrect)
{
    for (int testcase = 1; testcase <= numCase; testcase++)
    {
        Input input = parseInput();
        vector<bool> infected(input.nNodes, false);
        cerr << "   infection chance: " << input.infectionChance << endl;
        float nAverageUpperboundInfected = (float)input.maxInfected / (float)input.nNodes;
        float nAverageLowerboundInfected = (float)input.minInfected / (float)input.nNodes;
        float averageInfectionRate = (float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / 2;

        // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
        float p = (float)averageInfectionRate + ((float)averageInfectionRate * (float)input.infectionChance);
        cerr << "   p(robabilty) sample is infected: " << p << endl;
        cerr << "   nodes: " << input.nNodes << endl;
        cerr << "   average infection rate: " << averageInfectionRate << endl;
        cerr << "   number of edges: " << input.nEdges << endl;
        nTests = 0;
        infectedFound = 0;
        nonInfectedFound = 0;
        AdjacencyMatrix adjMatrix = createAdjacencyMatrix(input);
        // if (averageInfectionRate >= 0.3)
        // {
        //     vvi base;
        //     vi base1;
        //     for (size_t i = 0; i < input.nNodes; i++)
        //     {

        //         base1.push_back(i);
        //     }
        //     base.push_back(base1);
        //     manualTest(base, infected, input);
        // }
        // else if (input.infectionChance <= 0.1 && averageInfectionRate < 0.1)
        // {
        //     cerr << "METHOD: Split" << endl;
        //     vi nodes;
        //     for (size_t i = 0; i < input.nNodes; i++)
        //     {
        //         nodes.push_back(i);
        //     }

        //     runTestCaseSpreadGraph(nodes, infected, input);
        // }
        // else if (averageInfectionRate > 0.2 || input.infectionChance < 0.3)
        // {
        vi nodes;

        vector<vector<int>> pairs;

        pairs = adjMatrix.findDenseSubGraph(0, pairs, 0);
        // pairs = adjMatrix.bfs(0);
        std::sort(pairs.begin(), pairs.end(), [](const vector<int> &a, const vector<int> &b) { return a.size() > b.size(); });

        for (size_t i = 0; i < pairs.size(); i++)
        {
            for (size_t j = 0; j < pairs.at(i).size(); j++)
            {
                nodes.push_back(pairs.at(i).at(j));
            }
        }
        // }
        // else
        // {
        //     for (size_t i = 0; i < input.nNodes; i++)
        //     {
        //         nodes.push_back(i);
        //     }
        // }

        cerr << "METHOD: Group" << endl;
        int k = calculateK(input);
        runTestCaseGrouped(k, nodes, infected, input);
        // }
        // else
        // {
        //     cerr << "METHOD: Coherant group" << endl;
        //     runTestCaseGroupedGraph(adjMatrix, infected, input);
        // }

        answerTestCase(infected, input.nNodes);

        // read in the result and show it to the user
        string result;
        cin >> result;
        cerr << "Test case " << testcase << ": " << result << endl;
        if (result == "success")
        {
            numCorrect++;
        }
        else
        {
            for (int i = 0; i < infected.size(); i++)
            {
                cerr << i << ": " << infected.at(i) << endl;
            }
        }
        cerr << "Number of tests used for: " << nTests << " for a total of nodes: " << input.nNodes << endl;
        cerr << "Number of predicited infected for: " << input.maxInfected << " actual # of infected: " << infectedFound << endl;
    }
}

int main()
{
    handleCredentials();

    int numCase;
    cin >> numCase;
    cerr << "number of test cases: " << numCase << endl
         << flush << endl;
    int numCorrect = 0;

    runTestCases(numCase, numCorrect);

    cerr << numCorrect << "/" << numCase << " correct" << endl;

    return 0;
}

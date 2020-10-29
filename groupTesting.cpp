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

int groupSize = 8;
int recursionThreshold = 10;
float thresholdRecursionStep = 0.1;

void recursiveTest(AdjacencyMatrix &adjMatrix, vvi inputPairs, vector<bool> &infected, float threshold)
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
        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold);
        manualTest(manualTestPairs, infected);
    }
}

vi customGroupTestSplit(vi nodes, vector<bool> &infected, int infectedCounter, int maxInfected)
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

    for (int i = 0; i < size; i++)
    {
        if (result == true && size == 1)
        {
            infectedCounter++;
        }
        infected[nodes.at(i)] = result;
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

void runTestCaseGrouped(int groupSize, vi nodes, vector<bool> &infected, int maxInfected)
{
    int size = groupSize;
    int currentIndex = 0;
    int infectedCounter = 0;
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
        vi result = customGroupTestSplit(subgroup, infected, infectedCounter, maxInfected);

        currentIndex = currentIndex + size;
        if (infectedCounter == maxInfected)
        {
            return;
        }
        if (result.size() > 2)
        {
            pairs.push_back(result);
        }
    }

    manualTest(pairs, infected);
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
        if (remainingTestsAreNegative(input)) {
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

        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold);
        manualTest(manualTestPairs, infected);
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
        cerr << "   nodes: " << input.nNodes << endl;
        cerr << "   average infection rate: " << averageInfectionRate << endl;
        cerr << "   number of edges: " << input.nEdges << endl;
        nTests = 0;
        infectedFound = 0;
        nonInfectedFound = 0;
        AdjacencyMatrix adjMatrix = createAdjacencyMatrix(input);

        if (averageInfectionRate >= 0.2)
        {
            vi nodes;
            if (input.infectionChance >= 0.3)
            {
                vector<vector<int>> pairs;

                pairs = adjMatrix.findDenseSubGraph(0, pairs, 0);

                for (size_t i = 0; i < pairs.size(); i++)
                {
                    for (size_t j = 0; j < pairs.at(i).size(); j++)
                    {
                        nodes.push_back(pairs.at(i).at(j));
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < input.nNodes; i++)
                {
                    nodes.push_back(i);
                }
            }

            cerr << "METHOD: Group" << endl;
            runTestCaseGrouped(groupSize, nodes, infected, input.maxInfected);
        }
        else if (input.nEdges < input.nNodes || input.infectionChance < 0.1) // (input.infectionChance < 0.1 && averageInfectionRate < 0.15)
        {
            vi nodes;
            for (size_t i = 0; i < input.nNodes; i++)
            {
                nodes.push_back(i);
            }

            runTestCaseSpreadGraph(nodes, infected, input);
        }
        else
        {
            cerr << "METHOD: Grouped" << endl;
            runTestCaseGroupedGraph(adjMatrix, infected, input);
        }

        answerTestCase(infected, input.nNodes);

        // read in the result and show it to the user
        string result;
        cin >> result;
        cerr << "Test case " << testcase << ": " << result << endl;
        if (result == "success") {
            numCorrect++;
        } else {
            for (int i = 0; i < infected.size(); i++) {
                cerr << i << ": " << infected.at(i) << endl;
            }
        }
        cerr << "Number of tests used for: " << nTests << " for a total of nodes: " << input.nNodes << endl;
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

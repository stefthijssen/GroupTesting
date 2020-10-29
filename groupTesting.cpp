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

#include "structs/input.h"

using namespace std;

float treshold = -0.1;

int recursionThreshold = 10;
float thresholdRecursionStep = 0.1;

void recursiveTest(AdjacencyMatrix &adjMatrix, vvi inputPairs, vector<bool> &infected, float threshold)
{
    cerr << "recursive test: " << inputPairs.size() << ", threshold: " << threshold << endl;
    unvisitPairsInMatrix(inputPairs, adjMatrix);
    vector<vector<int>> pairs;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, threshold);
    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testGraph(pairs.at(i));
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
        cerr << "newThreshold: " << newThreshold << endl;
        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold);
        manualTest(manualTestPairs, infected);
    }
}

void runTestCaseSpreadGraph(vi nodes, vector<bool> &infected, int maxInfected) {
    int middle = nodes.size()/2;
    int infectedCounter = 0;

    cerr << maxInfected << endl;

    splitTest(nodes, 0, middle, infected, infectedCounter, maxInfected, true);
    splitTest(nodes, middle, nodes.size(), infected, infectedCounter, maxInfected, false);
}

void runTestCaseGroupedGraph(AdjacencyMatrix &adjMatrix, vector<bool> &infected)
{
    vector<vector<int>> pairs;
    int minGroups = 2;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, baseThreshold);

    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testGraph(pairs.at(i));
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

        AdjacencyMatrix adjMatrix = createAdjacencyMatrix(input);

        if (input.nEdges < input.nNodes || input.infectionChance < 0.1) {
            vi nodes;
            for (size_t i = 0; i < input.nNodes; i++) {
                nodes.push_back(i);
            }

            runTestCaseSpreadGraph(nodes, infected, input.maxInfected);
        } else {
            runTestCaseGroupedGraph(adjMatrix, infected);
        }

        answerTestCase(infected, input.nNodes);

        // read in the result and show it to the user
        string result;
        cin >> result;
        cerr << "Test case " << testcase << ": " << result << endl;
        if (result == "success")
            numCorrect++;
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

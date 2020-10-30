#include <iostream>
#include <fstream>
#include <vector>

#include "utils/credentials.cpp"
#include "utils/inputParser.cpp"
#include "utils/algorithmCase.cpp"

#include "algorithms/splitTest.cpp"
#include "algorithms/poolTest.cpp"
#include "algorithms/oneByOneTest.cpp"

#include "headers/input.hpp"
#include "headers/global.hpp"

using namespace std;

void resetGlobals() {
    nTests = 0;
    infectedFound = 0;
    nonInfectedFound = 0;
}

void giveFeedbackOnTest(Input input, int testcase, int &numCorrect, vector<bool> infected) {
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
}


void runTestCases(int numCase, int &numCorrect)
{
    for (int testcase = 1; testcase <= numCase; testcase++)
    {
        Input input = parseInput();
        vector<bool> infected(input.nNodes, false);

        resetGlobals();

        float averageInfectionRate = ((float)(input.maxInfected + input.minInfected) / 2) / (float)input.nNodes;

        // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
        float p = calculateP(input);

        cerr << endl << endl;
        cerr << "infection chance: " << input.infectionChance << endl;
        cerr << "p(robabilty) sample is infected: " << p << endl;
        cerr << "nodes: " << input.nNodes << endl;
        cerr << "average infection rate: " << averageInfectionRate << endl;
        cerr << "number of edges: " << input.nEdges << endl;
        cerr << "number of infected: " << input.minInfected << " - " << input.maxInfected << endl;
        
        AdjacencyMatrix adjMatrix = createAdjacencyMatrix(input);

        Algorithm algorithmCase = checkWhichAlgorithmToUse(input, calculateP(input));

        switch (algorithmCase) {
            case split:
                useSplitTest(input, infected);
                break;
            case pool:
                usePoolTest(input, adjMatrix, infected);
                break;
            case oneByOne:
                useOneByOne(input, infected);
                break;
            default:
                useOneByOne(input, infected);
                break;
        }

        answerTestCase(infected, input.nNodes);

        giveFeedbackOnTest(input, testcase, numCorrect, infected);
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

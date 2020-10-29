#include <iostream>
#include <fstream>
#include <vector>

#include "../headers/input.h"
#include "../headers/global.h"

using namespace std;

void testNodes(vi nodes)
{
    cout << "test";
    for (size_t j = 0; j < nodes.size(); j++)
    {
        cout << " ";
        cout << nodes.at(j);
    }

    cout << endl
         << flush << endl;
    nTests = nTests + 1;
}

bool retrieveTestResult()
{
    string result;
    cin >> result;
    if (result == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

void answerTestCase(vector<bool> &infected, int nNodes)
{
    cout << "answer ";
    bool first = true;
    for (int i = 0; i < nNodes; i++)
    {
        if (infected[i])
        {
            if (not first)
            {
                cout << " ";
            }
            cout << i;
            first = false;
        }
    }
    cout << endl
         << flush;
}

void manualTest(vvi toTest, vector<bool> &infected)
{
    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            cout << "test " << toTest.at(i).at(j) << endl
                 << flush << endl;
            nTests = nTests + 1;
        }
    }

    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            bool result = retrieveTestResult();

            if (result == false) {
                nonInfectedFound++;
            } else {
                infectedFound++;
            }

            infected[toTest.at(i).at(j)] = result;
        }
    }
}

void updateInfected(vi subgraph, vector<bool> &infected, vvi &toTest)
{
    bool result = retrieveTestResult();

    if (result == true)
    {
        if (subgraph.size() > 1) {
            toTest.push_back(subgraph);
        } else {
            infectedFound++;
        }
    }

    for (int j = 0; j < subgraph.size(); j++)
    {
        infected[subgraph.at(j)] = result;
    }
}

bool remainingTestsAreNegative(Input input) {
    return infectedFound >= input.maxInfected;
}

bool remainingTestsArePositive(Input input) {
    return (input.nNodes - nonInfectedFound) <= input.minInfected;
}
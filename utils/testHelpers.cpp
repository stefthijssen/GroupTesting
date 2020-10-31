#include <iostream>
#include <fstream>
#include <vector>

#include "../headers/input.hpp"
#include "../headers/global.hpp"

#pragma once

using namespace std;

bool remainingTestsAreNegative(Input input)
{
    bool remainingAreNegative = infectedFound >= input.maxInfected;
    if (remainingAreNegative == true) {
        cerr << "I am done found maximum infected " << infectedFound << endl;
    }
    return remainingAreNegative;
}

bool remainingTestsArePositive(Input input)
{
    bool remainingIsPositive = (input.nNodes - nonInfectedFound) <= input.minInfected;
    if (remainingIsPositive == true) {
        cerr << "I am done found maximum nonInfected " << nonInfectedFound << endl;
    }
    return remainingIsPositive;
}

void testNode(int node)
{
    cout << "test " << node << endl
         << flush << endl;
    nTests = nTests + 1;
}

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

void updateInfected(vi subgraph, vector<bool> &infected, vvi &toTest)
{
    bool result = retrieveTestResult();

    if (result == true)
    {
        if (subgraph.size() > 1)
        {
            toTest.push_back(subgraph);
        }
        else
        {
            infectedFound++;
        }
    }
    else if (result == false && subgraph.size() == 1)
    {
        nonInfectedFound++;
    }

    for (int j = 0; j < subgraph.size(); j++)
    {
        infected[subgraph.at(j)] = result;
    }
}

bool oneByOneTest(vi toTest, vector<bool> &infected, Input input)
{
    bool remainingNegative = false;
    bool remainingPostive = false;
    int at;
    for (size_t i = 0; i < toTest.size(); i++)
    {
        testNode(toTest.at(i));

        bool result = retrieveTestResult();
        infected[toTest.at(i)] = result;

        if (result == true)
        {
            infectedFound++;
        }
        else
        {
            nonInfectedFound++;
        }

        if (remainingTestsAreNegative(input))
        {
            at = i;
            remainingNegative = true;
            break;
        } else if (remainingTestsArePositive(input)) {
            at = i;
            remainingPostive = true;
            break;
        }
    }

    if (remainingNegative == true)
    {
        int startIndex = toTest.at(at) + 1;
        for (size_t i = startIndex; i < infected.size(); i++)
        {
            infected[i] = false;
            nonInfectedFound++;
        }
        return true;
    } else if (remainingPostive == true) {
        int startIndex = toTest.at(at) + 1;
        for (size_t i = startIndex; i < infected.size(); i++)
        {
            infected[i] = true;
            infectedFound++;
        }
        return true;
    }
    return false;
}
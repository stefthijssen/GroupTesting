#include <iostream>
#include <fstream>
#include <vector>

#include "../headers/input.hpp"
#include "../headers/global.hpp"

#pragma once

using namespace std;

bool remainingTestsAreNegative(Input input)
{
    return infectedFound >= input.maxInfected;
}

bool remainingTestsArePositive(Input input)
{
    return (input.nNodes - nonInfectedFound) == input.maxInfected;
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
    bool done = false;
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
            cerr << "I am done found maximum infected " << infectedFound << " " << input.maxInfected << endl;
            at = i;
            done = true;
            break;
        }
    }

    if (done)
    {
        for (size_t i = at + 1; i < toTest.size(); i++)
        {
            infected[toTest.at(i)] = false;
            nonInfectedFound++;
        }
        return true;
    }
    return false;
}
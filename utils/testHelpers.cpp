#include <iostream>
#include <fstream>
#include <vector>

#include "../headers/input.h"
#include "../headers/global.h"

using namespace std;

bool remainingTestsAreNegative(Input input)
{
    return infectedFound >= input.maxInfected;
}

bool remainingTestsArePositive(Input input)
{
    return (input.nNodes - nonInfectedFound) <= input.minInfected;
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

void oneByOneTest(vi toTest, vector<bool> &infected, Input input)
{
    bool done = false;
    int at;
    for (size_t i = 0; i < toTest.size(); i++)
    {
        cout << "test " << toTest.at(i) << endl
             << flush << endl;
        nTests = nTests + 1;

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
        for (size_t i = at+1; i < toTest.size(); i++)
        {
            infected[toTest.at(i)] = false;
        }
    }
}

void manualTest(vvi toTest, vector<bool> &infected, Input input)
{
    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            cout << "test " << toTest.at(i).at(j) << endl
                 << flush << endl;
            nTests = nTests + 1;

            bool result = retrieveTestResult();
            infected[toTest.at(i).at(j)] = result;

            if (result == true)
            {
                infectedFound++;
            }
            else
            {
                nonInfectedFound++;
            }
        }

        if (remainingTestsAreNegative(input))
        {
            cerr << "I am done found maximum infected " << infectedFound << " " << input.maxInfected << endl;
            return;
        }
    }
}

#include <iostream>
#include <fstream>
#include <vector>

#include "../structs/input.h"

using namespace std;

void testGraph(vi pair)
{
    /* DEBUG */
    cerr << " Number of nodes per subgraph: " << pair.size() << endl
         << flush;
    /* DEBUG */

    cout << "test";
    for (size_t j = 0; j < pair.size(); j++)
    {
        /* DEBUG */
        // cerr << "At: " << pair.at(j) << endl << flush;
        /* DEBUG */

        cout << " ";
        cout << pair.at(j);
    }

    cout << endl
         << flush << endl;

    /* DEBUG */
    // cerr << pair.at(0) << endl << flush << endl;
    /* DEBUG */
}

bool retrieveTestResult() {
    string result;
    cin >> result;
    if (result == "true") {
        return true;
    } else {
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
        }
    }

    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            string result;
            cin >> result;
            bool boolResult;
            if (result == "true")
            {
                boolResult = true;
            }
            else
            {
                boolResult = false;
            }

            infected[toTest.at(i).at(j)] = boolResult;
        }
    }
}

void updateInfected(vi subgraph, vector<bool> &infected, vvi &toTest)
{
    bool result = retrieveTestResult();

    if (result == true && subgraph.size() > 1) {
        toTest.push_back(subgraph);
    }

    for (int j = 0; j < subgraph.size(); j++)
    {
        infected[subgraph.at(j)] = result;
    }
}
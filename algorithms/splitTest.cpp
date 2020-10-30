#include <iostream>
#include <fstream>
#include <vector>

#include "../headers/input.hpp"
#include "../headers/global.hpp"

#include "../utils/testHelpers.cpp"

using namespace std;

void splitTest(vi nodes, int left, int right, vector<bool> &infected, Input input)
{
    bool testResult;
    int size = right - left;
    int middle = (size / 2) + left;

    if (remainingTestsArePositive(input)) {
        testResult = true;
    } else if (remainingTestsAreNegative(input)) {
        testResult = false;
    } else {
        cout << "test";
        for (int i = 0; i < size; i++)
        {
            cout << " " << left + i;
        }

        cout << endl
            << flush << endl;
        nTests = nTests + 1;
        testResult = retrieveTestResult();
    }

    if (testResult == true && size == 1)
    {
        infectedFound++;
    }

    for (int i = 0; i < size; i++)
    {
        if (testResult == false) {
            nonInfectedFound++;
        }

        infected[left + i] = testResult;
    }

    if (size < 2 || testResult == false)
    {
        return;
    }

    splitTest(nodes, left, middle, infected, input);
    splitTest(nodes, middle, right, infected, input);
}

void useSplitTest(Input input, vector<bool> &infected) {
    vi nodes;
    for (size_t i = 0; i < input.nNodes; i++)
    {
        nodes.push_back(i);
    }

    int middle = nodes.size() / 2;

    splitTest(nodes, 0, middle, infected, input);
    splitTest(nodes, middle, nodes.size(), infected, input);
}
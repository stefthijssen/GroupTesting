#include <vector>

#include "../headers/input.hpp"
#include "../utils/testHelpers.cpp"

using namespace std;

void useOneByOne(Input input, vector<bool> &infected) {
    vi nodes;
    for (size_t i = 0; i < input.nNodes; i++)
    {
        nodes.push_back(i);
    }

    ReturnStatus status = oneByOneTest(nodes, infected, input);

    if (status.first == NegativeRemaining)
    {
        for (size_t i = status.second; i < nodes.size(); i++)
        {
            infected[i] = false;
            nonInfectedFound++;
        }
    }
    else if (status.first == PositiveRemaining) {
        for (size_t i = status.second; i < nodes.size(); i++)
        {
            infected[nodes.at(i)] = true;
            infectedFound++;
        }
    }
}
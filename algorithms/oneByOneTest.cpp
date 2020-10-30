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

    oneByOneTest(nodes, infected, input);
}
#include "headers/input.h"

using namespace std;

enum Algorithm {
    split,
    pool,
    oneByOne
};

Algorithm checkWhichAlgorithmToUse(Input input) {
    float maxInfectedFactor = ((float) input.maxInfected / (float) input.nNodes);
    float averageAmountInfected = ((float) (input.maxInfected + input.minInfected)) / 2;
    float amountInfectedFactor = (averageAmountInfected / (float) input.nNodes);

    if (maxInfectedFactor <= 0.02) {
        return split;
    }

    if (amountInfectedFactor >= 0.5) {
        return oneByOne;
    }

    return pool;
}
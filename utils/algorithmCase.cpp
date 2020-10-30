#include "../headers/input.h"
#include "formulas.cpp"

using namespace std;

enum Algorithm {
    split,
    pool,
    oneByOne,
    poolHighInfection
};

Algorithm checkWhichAlgorithmToUse(Input input) {
    float maxInfectedFactor = ((float) input.maxInfected / (float) input.nNodes);
    float averageAmountInfected = ((float) (input.maxInfected + input.minInfected)) / 2;
    float amountInfectedFactor = (averageAmountInfected / (float) input.nNodes);

    if (maxInfectedFactor <= 0.02) {
        cerr << "METHOD: Split" << endl;
        return split;
    }

    if (amountInfectedFactor >= 0.8) { // Tweak?
        cerr << "METHOD: oneByOne" << endl;
        return oneByOne;
    }

    if (calculateP(input) >= 0.45) {
        cerr << "METHOD: Test and group" << endl;
        return poolHighInfection;
    }

    cerr << "METHOD: Pool" << endl;
    return pool;
}
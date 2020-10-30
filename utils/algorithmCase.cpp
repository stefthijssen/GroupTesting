#include "../headers/input.h"
#include "formulas.cpp"

using namespace std;

enum Algorithm {
    split,
    pool,
    oneByOne
};

Algorithm checkWhichAlgorithmToUse(Input input) {
    if (calculateP(input) <= 0.03) {
        cerr << "METHOD: Split" << endl;
        return split;
    }

    if (calculateP(input) >= 0.45) {
        cerr << "METHOD: oneByOne" << endl;
        return oneByOne;
    }

    cerr << "METHOD: Pool" << endl;
    return pool;
}
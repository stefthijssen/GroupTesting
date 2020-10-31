#include "../headers/input.hpp"

using namespace std;

enum Algorithm {
    split,
    pool,
    oneByOne
};

Algorithm checkWhichAlgorithmToUse(Input input, float p) {
    if (p <= 0.04) {
        cerr << "METHOD: Split" << endl;
        return split;
    }

    if (p >= 0.8) {
        cerr << "METHOD: oneByOne" << endl;
        return oneByOne;
    }

    cerr << "METHOD: Pool" << endl;
    return pool;
}
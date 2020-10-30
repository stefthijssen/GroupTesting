#include "../headers/input.hpp"

using namespace std;

enum Algorithm {
    split,
    pool,
    oneByOne
};

Algorithm checkWhichAlgorithmToUse(Input input, float p) {
    if (p <= 0.03) {
        cerr << "METHOD: Split" << endl;
        return split;
    }

    if (p >= 0.75) {
        cerr << "METHOD: oneByOne" << endl;
        return oneByOne;
    }

    cerr << "METHOD: Pool" << endl;
    return pool;
}
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void splitTest(vi nodes, int left, int right, vector<bool> &infected, int &infectedCounter, int maxInfected, bool isLeft) {
    if (infectedCounter > maxInfected) {
        return;
    }

    int size = right - left;
    int middle = (size/2)+left;

    cout << "test";
    for (int i = 0; i < size; i++) {
        cout << " " << left+i;
    }

    cout << endl << flush << endl;

    bool result = retrieveTestResult();

    for (int i = 0; i < size; i++) {
        if (result == true && size == 1) {
            infectedCounter++;
        }
        infected[left+i] = result;
    }
    
    if (size < 2 || result == false) {
        return;
    }

    splitTest(nodes, left, middle, infected, infectedCounter, maxInfected, true);
    splitTest(nodes, middle, right, infected, infectedCounter, maxInfected, false);
}
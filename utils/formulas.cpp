#include "../headers/input.h"
#include "../headers/global.h"

using namespace std;

float calculateP(Input input)
{
    float nAverageUpperboundInfected = (float)input.maxInfected - infectedFound;
    float nAverageLowerboundInfected = (float)input.minInfected - infectedFound;
    float averageInfectionRate = (float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / ((input.nNodes - nonInfectedFound) * 2);

    // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
    float p = (float)averageInfectionRate + ((float)averageInfectionRate * (float)input.infectionChance);
    return p;
}

int calculateK(Input input)
{
    //default to k = 8
    int k = 8;
    float p = calculateP(input);
    if (p >= 0.15)
    {
        k = 3;
    }
    else if (p >= 0.1)
    {
        k = 4;
    }
    else if (p >= 0.05)
    {
        k = 7;
    }
    else if (p >= 0.01)
    {
        k = 11;
    }
    return k;
}
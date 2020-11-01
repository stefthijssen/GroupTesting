#include "../headers/input.hpp"
#include "../headers/global.hpp"

#pragma once

using namespace std;
float calculateInfectionRate(Input input)
{
    float nAverageUpperboundInfected = (float)input.maxInfected - infectedFound;
    float nAverageLowerboundInfected = (float)input.minInfected - infectedFound;
    float averageInfectionRate = ((float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / 2) / (input.nNodes - (nonInfectedFound + infectedFound));
    return averageInfectionRate;
}

float calculateP(Input input)
{
    float averageInfectionRate = calculateInfectionRate(input);
    float spreadFactor = (float)input.infectionChance * (input.nEdges / input.nNodes);

    // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
    float p;
    p = (float)averageInfectionRate + ((float)averageInfectionRate * spreadFactor);

    return p;
}

int calculateK(Input input, float p)
{
    //default to k = 8
    int k = 8;
    if (p >= 0.3)
    {
        k = 2;
    }
    else if (p >= 0.15)
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
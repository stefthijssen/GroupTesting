#include "../headers/input.hpp"
#include "../headers/global.hpp"

#pragma once

using namespace std;

float calculateP(Input input)
{
    float nAverageUpperboundInfected = (float)input.maxInfected - infectedFound;
    float nAverageLowerboundInfected = (float)input.minInfected - infectedFound;
    float nAverageInfectedNodes = (float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / 2;
    float averageInfectionRate = nAverageInfectedNodes / (input.nNodes - (nonInfectedFound + infectedFound));

    return averageInfectionRate;
}

int calculateK(Input input, float p)
{
    int k;
    if (p >= 0.25) // 0.3 optimal
    {
        k = 2;
    }
    else if (p >= 0.13) // 0.15 optimal
    {
        k = 3;
    }
    else if (p >= 0.08) // 0.1 optimal
    {
        k = 4;
    }
    else if (p >= 0.045) // 0.05 optimal
    {
        k = 5;
    }
    else if (p >= 0.022) // 0.025 optimal
    {
        k = 7;
    }
    else // 0.01 optimal
    {
        k = 11;
    }
    return k;
}
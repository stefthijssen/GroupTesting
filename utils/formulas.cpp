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
    float spreadFactor = (float)input.infectionChance;
    float contactFactor = 0;

    if (input.nEdges > 0 && input.nNodes > 0)
    {
        contactFactor = (input.nEdges / input.nNodes) * averageInfectionRate;
        spreadFactor = spreadFactor * contactFactor;
    }
    // P the chance a sample is positive increases when average infection rate is high and infection chacnce is also high.
    float p;
    p = (float)averageInfectionRate;

    return p;
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
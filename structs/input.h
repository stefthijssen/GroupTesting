#include <vector>

#ifndef INPUT_HPP_
#define INPUT_HPP_

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;

struct Input
{
    int nNodes;
    int nEdges;
    int nInfected;
    long double infectionChance;
    int minInfected;
    int maxInfected;
    vvi graph;
    vii edges;
};

#endif
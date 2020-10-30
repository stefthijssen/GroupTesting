#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <numeric>
#include <math.h>

#include "../headers/input.hpp"
#include "../headers/global.hpp"

using namespace std;

float baseThreshold = -0.1;

class AdjacencyMatrix
{
private:
    int n;
    Input input;
    int **adj;
    bool *visited;
    vector<int> outgoingEdges;
    void resetVisited()
    {
        for (int i = 0; i < n; i++)
            visited[i] = false;
    }
    int arraySize(int arr[])
    {
        int arrSize = *(&arr + 1) - arr;
        return arrSize;
    }
    int arraySize(float arr[])
    {
        int arrSize = *(&arr + 1) - arr;
        return arrSize;
    }

public:
    AdjacencyMatrix() {}
    AdjacencyMatrix(int n, Input input)
    {
        this->n = n;
        this->input = input;
        visited = new bool[n];
        adj = new int *[n];
        vector<pair<int,int>> tmpOutgoingEdges = vector<pair<int,int>>(n);
        for (size_t i = 0; i < n; i++)
        {
            adj[i] = new int[n];
            int edgeCounter = 0;
            for (size_t j = 0; j < n; j++)
            {
                adj[i][j] = 0;
                if (i == j)
                {
                    adj[i][j] = 1;
                    edgeCounter++;
                }
            }
            tmpOutgoingEdges[i] = pair<int,int>(i,edgeCounter);
        }

        std::sort(tmpOutgoingEdges.begin(), tmpOutgoingEdges.end(), [](const pair<int,int> &a, const pair<int,int> &b) { return a.second > b.second; });
        for (size_t i = 0; i < tmpOutgoingEdges.size(); i++)
        {
            outgoingEdges.push_back(tmpOutgoingEdges[i].first);
        }
        

        resetVisited();
    }
    int outgoingEdgesArray()
    {
        return distance(outgoingEdges.begin(), max_element(outgoingEdges.begin(), outgoingEdges.end()));
    }
    int size()
    {
        return n * n;
    }
    void addEdge(int u, int v)
    {
        adj[u][v] = 1;
        adj[v][u] = 1;
    }
    void dfs(int src)
    {
        visited[src] = true;
        for (size_t i = 0; i < n; i++)
        {
            if (adj[src][i] == 1 && (!visited[i]))
            {
                dfs(i);
            }
        }
    }
    void setVisited(int n, bool value)
    {
        visited[n] = value;
    }
    vector<vector<int>> findDenseSubGraph(int currentIndex, vector<vector<int>> pairs, float threshold)
    {

        if (currentIndex >= n)
        {
            return pairs;
        }
        if (visited[currentIndex])
        {
            int next = currentIndex + 1;
            return findDenseSubGraph(next, pairs, threshold);
        }
        visited[currentIndex] = true;

        // float average = accumulate(difference.begin(), difference.end(), 0.0) / n;

        // float acceptable = average;
        // cerr << "min infected: " << input.minInfected << endl;
        // cerr << "max infected: " << input.maxInfected << endl;
        // cerr << "N infected: " << input.nInfected << endl;
        // cerr << "chance infected: " << input.infectionChance << endl;

        float nAverageUpperboundInfected = (float)input.maxInfected / (float)n;
        float nAverageLowerboundInfected = (float)input.minInfected / (float)n;
        float averageInfectionRate = (float)(nAverageUpperboundInfected + nAverageLowerboundInfected) / 2;
        // // cerr << averageInfectionRate << endl;
        // // cerr << input.infectionChance << endl;
        float acceptable = 1 - input.infectionChance + threshold - averageInfectionRate; // Higher means less difference will be accepted
        // float acceptable = 0.3;

        vector<int> accepted;
        accepted.push_back(currentIndex);
        int next = currentIndex + 1;

        for (size_t i = 0; i < n; i++)
        {
            float diff = calcDifference(adj[currentIndex], adj[i]);
            // cerr << "At: " << i << "Difference: " << diff << endl;

            if (i == currentIndex)
                continue;
            // cerr << diff << endl;
            if (diff <= acceptable && visited[i] == false)
            {
                visited[i] = true;
                accepted.push_back(i);
            }
            int bestNextCandidate = outgoingEdges[i];
            if (visited[bestNextCandidate] == false)
            {
                next = bestNextCandidate;
            }
        }

        pairs.push_back(accepted);
        return findDenseSubGraph(next, pairs, baseThreshold);
    }

    float calcDifference(int arr1[], int arr2[])
    {
        if (input.nEdges < 1)
        {
            return 1;
        }
        float averageGraphDensity = ceil(input.nEdges / n);
        float diff = averageGraphDensity * 2;
        for (size_t i = 0; i < n; i++)
        {
            if (arr1[i] == arr2[i] && arr1[i] == 1)
            {
                diff--;
            }
        }
        // cerr << averageGraphDensity << diff << endl;
        float answer = diff / (averageGraphDensity * 2);
        return answer;
    }
    vector<vector<int>> bfs(int src)
    {
        // resetVisited();
        queue<int> q;
        vector<vector<int>> r;

        // Initialize source
        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            int current = q.front();
            // cerr << current << endl;
            vector<int> rs;
            rs.push_back(current);
            visited[current] = true;

            for (size_t i = 0; i < n; i++)
            {
                if (adj[current][i] == 1 && (visited[i] == false))
                {
                    visited[i] = true;
                    rs.push_back(i);
                    q.push(i);
                }
            }

            q.pop();
            r.push_back(rs);
            if (q.empty())
            {
                for (size_t i = 0; i < n; i++)
                {
                    if (visited[i] == false)
                    {
                        q.push(i);
                        break;
                    }
                }
            }
        }
        return r;
    }
    void display()
    {
        int i, j;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                cerr << adj[i][j] << " ";
            }
            cerr << endl
                 << flush << endl;
        }
    }
};

AdjacencyMatrix createAdjacencyMatrix(Input input)
{
    AdjacencyMatrix adjMatrix;
    (&adjMatrix)->~AdjacencyMatrix();
    new (&adjMatrix) AdjacencyMatrix(input.nNodes, input);

    for (size_t i = 0; i < input.edges.size(); i++)
    {
        int a, b;
        a = input.edges.at(i).first;
        b = input.edges.at(i).second;
        adjMatrix.addEdge(a, b);
    }

    return adjMatrix;
}

void unvisitPairsInMatrix(vvi pairs, AdjacencyMatrix &adjMatrix)
{
    for (size_t i = 0; i < pairs.size(); i++)
    {
        for (size_t j = 0; j < pairs.at(i).size(); j++)
        {
            adjMatrix.setVisited(pairs.at(i).at(j), false);
        }
    }
}
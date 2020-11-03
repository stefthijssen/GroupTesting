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
    int **degree;
    float **affinity;
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
        affinity = new float *[n];
        degree = new int *[n];
        vector<pair<int, int>> tmpOutgoingEdges = vector<pair<int, int>>(n);
        for (size_t i = 0; i < n; i++)
        {
            adj[i] = new int[n];
            affinity[i] = new float[n];
            degree[i] = new int[n];
            for (size_t j = 0; j < n; j++)
            {
                adj[i][j] = 0;
                degree[i][j] = 0;
                affinity[i][j] = 0;
                if (i == j)
                {
                    adj[i][j] = 1;
                }
            }
        }

        resetVisited();
    }
    int findMinIndex(int **arr)
    {
        int currentMinVal = input.nEdges + 1;
        int resultIndex = -1;
        for (size_t i = 0; i < n; i++)
        {
            if (arr[i][i] < currentMinVal && visited[i] == false)
            {
                currentMinVal = arr[i][i];
                resultIndex = i;
            }
        }
        return resultIndex;
    }
    int findMaxIndex(int **arr)
    {
        int currentMaxVal = -1;
        int resultIndex = -1;
        for (size_t i = 0; i < n; i++)
        {
            if (arr[i][i] > currentMaxVal && visited[i] == false)
            {
                currentMaxVal = arr[i][i];
                resultIndex = i;
            }
        }
        return resultIndex;
    }
    vector<int> maxSubArray(float *arr)
    {
        float currentMax = -1;
        vector<int> maxArray;
        for (size_t i = 0; i < n; i++)
        {
            // cerr << arr[i] << endl;
            if (visited[i] == true)
            {
                continue;
            }
            if (currentMax < arr[i])
            {
                maxArray.clear();
                maxArray.push_back(i);
                currentMax = arr[i];
            }
            else if (currentMax == arr[i])
            {
                maxArray.push_back(i);
            }
        }
        return maxArray;
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
        degree[u][u]++;
        degree[v][v]++;
    }
    void setVisited(int n, bool value)
    {
        visited[n] = value;
    }
    vector<vector<int>> findDenseSubGraph(int currentIndex, vector<vector<int>> pairs)
    {
        currentIndex = findMaxIndex(degree);
        while (currentIndex != -1)
        {
            visited[currentIndex] = true;
            vi cluster;
            cluster = maxSubArray(affinity[currentIndex]);
            for (size_t i = 0; i < cluster.size(); i++)
            {
                visited[cluster.at(i)] = true;
            }
            cluster.push_back(currentIndex);

            pairs.push_back(cluster);

            currentIndex = findMaxIndex(degree);
        }
        // display();

        return pairs;
    }
    void calculateAffinity()
    {
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                affinity[i][j] = calcAffinity(adj[i], adj[j]);
            }
        }
    }
    float calcAffinity(int arr1[], int arr2[])
    {
        if (input.nEdges < 1)
        {
            return 0;
        }
        float max = n;
        float min = 0;
        for (size_t i = 0; i < n; i++)
        {
            if (arr1[i] == arr2[i] && arr1[i] == 1)
            {
                min++;
            }
        }
        // cerr << min << endl;
        // cerr << min / max << endl;
        return min / max;
    }
    void display()
    {
        int i, j;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                cerr << affinity[i][j] << " ";
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
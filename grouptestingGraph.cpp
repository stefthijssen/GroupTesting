#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
using namespace std;
float treshold = 0.7;
class AdjacencyMatrix
{
private:
    int n;
    int **adj;
    bool *visited;

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
    AdjacencyMatrix(int n)
    {
        this->n = n;
        visited = new bool[n];
        adj = new int *[n];
        for (size_t i = 0; i < n; i++)
        {
            adj[i] = new int[n];
            for (size_t j = 0; j < n; j++)
            {
                adj[i][j] = 0;
                if (i == j)
                {
                    adj[i][j] = 1;
                }
            }
        }
        resetVisited();
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
    vector<vector<int>> findDenseSubGraph(int currentIndex, vector<vector<int>> pairs)
    {

        if (currentIndex >= n)
        {
            return pairs;
        }
        if (visited[currentIndex])
        {
            int next = currentIndex + 1;
            return findDenseSubGraph(next, pairs);
        }
        visited[currentIndex] = true;

        float difference[n];

        for (size_t i = 0; i < n; i++)
        {
            if (i == currentIndex)
            {
                continue;
            }
            float diff = calcDifference(adj[currentIndex], adj[i]);
            difference[i] = diff;
        }
        
        // float average = accumulate(difference.begin(), difference.end(), 0.0) / n;

        // float acceptable = average;
        float acceptable = treshold;

        // acceptable = average + (average * treshold);

        vector<int> accepted;
        accepted.push_back(currentIndex);
        for (size_t i = 0; i < n; i++)
        {
            // cerr << "At: " << i << "Difference: " << difference[i] << endl;

            if (i == currentIndex)
                continue;
            if (difference[i] <= acceptable && visited[i] == false)
            {
                visited[i] = true;
                accepted.push_back(i);
            }
        }
        pairs.push_back(accepted);
        int next = currentIndex + 1;
        return findDenseSubGraph(next, pairs);
    }
    float calcDifference(int arr1[], int arr2[])
    {
        float diff = n;
        for (size_t i = 0; i < n; i++)
        {
            if (arr1[i] && arr2[i])
            {
                diff--;
            }
        }
        return diff/n;
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

int main()
{
    AdjacencyMatrix adjMatrix;
    (&adjMatrix)->~AdjacencyMatrix();
    new (&adjMatrix) AdjacencyMatrix(6);
    adjMatrix.addEdge(0, 1);
    adjMatrix.addEdge(0, 2);
    adjMatrix.addEdge(1, 0);
    adjMatrix.addEdge(1, 2);
    adjMatrix.addEdge(2, 0);
    adjMatrix.addEdge(2, 1);
    adjMatrix.addEdge(2, 5);
    adjMatrix.addEdge(4, 5);
    adjMatrix.addEdge(5, 4);

    vector<vector<int>> pairs;

    pairs = adjMatrix.findDenseSubGraph(0, pairs);

    for (size_t i = 0; i < pairs.size(); i++)
    {
        cerr << " Number of nodes per subgraph: " << pairs.at(i).size() << endl
             << flush;
        for (size_t j = 0; j < pairs.at(i).size(); j++)
        {
            cerr << "At: " << pairs.at(i).at(j) << endl
                 << flush;
        }
    }

    adjMatrix.display();

    return 0;
}

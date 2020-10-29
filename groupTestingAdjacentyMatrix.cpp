#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <tgmath.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef int64_t ll;

float treshold = -0.1;

float baseThreshold = -0.1;
int recursionThreshold = 10;
float thresholdRecursionStep = 0.1;

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

class AdjacencyMatrix
{
private:
    int n;
    Input input;
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
    AdjacencyMatrix(int n, Input input)
    {
        this->n = n;
        this->input = input;
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
    void setVisited(int n, bool value)
    {
        visited[n] = value;
    }
    vector<vector<int>> findDenseSubGraph(int currentIndex, vector<vector<int>> pairs, float threshold, int minGroups)
    {

        if (currentIndex >= n)
        {
            return pairs;
        }
        if (visited[currentIndex])
        {
            int next = currentIndex + 1;
            return findDenseSubGraph(next, pairs, threshold,minGroups);
        }
        visited[currentIndex] = true;

        // float average = accumulate(difference.begin(), difference.end(), 0.0) / n;

        // float acceptable = average;
        // cerr << "min infected: " << input.minInfected << endl;
        // cerr << "max infected: " << input.maxInfected << endl;
        // cerr << "N infected: " << input.nInfected << endl;
        // cerr << "chance infected: " << input.infectionChance << endl;

        float acceptable = 1 - input.infectionChance + threshold;
        // cerr << " Acceptable rate: " << acceptable << endl;
        // acceptable = average + (average * treshold);

        vector<int> accepted;
        accepted.push_back(currentIndex);
        int next;
        if (input.nEdges < n || input.infectionChance < 0.1)
        {
            for (size_t i = currentIndex; i < currentIndex + (n / minGroups); i++)
            {

                // cerr << "At: " << i << "Difference: " << difference[i] << endl;
                if (i == currentIndex || visited[i])
                    continue;
                visited[i] = true;
                accepted.push_back(i);
            }

            next = currentIndex + (n / minGroups);
        }
        else
        {
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
            }

            next = currentIndex + 1;
        }
        pairs.push_back(accepted);
        return findDenseSubGraph(next, pairs, baseThreshold, minGroups);
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
    void bfs(int src)
    {
        resetVisited();
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
            for (size_t i = 0; i < n; i++)
            {
                if (adj[current][i] == 1 && (!visited[i]))
                {
                    rs.push_back(i);
                    q.push(i);
                }
            }
            q.pop();
            r.push_back(rs);
        }
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

// Parses the cin into the Input struct
Input parseInput()
{
    int nNodes, nEdges, nInfected;
    cin >> nNodes >> nEdges >> nInfected;
    float infectionChance;
    cin >> infectionChance;
    int minInfected, maxInfected;
    cin >> minInfected >> maxInfected;

    vvi graph(nEdges);
    vii edges(nEdges);

    for (int i = 0; i < nEdges; i++)
    {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
        ii edge1 = {a, b};
        ii edge2 = {b, a};
        edges.push_back(edge1);
        edges.push_back(edge2);
    }

    pair<vvi, vii> graphAndEdges = {graph, edges};

    vvi graphAndEdgesFirst = graphAndEdges.first;
    vii graphAndEdgesSecond = graphAndEdges.second;

    Input input = {
        nNodes,
        nEdges,
        nInfected,
        infectionChance,
        minInfected,
        maxInfected,
        graphAndEdgesFirst,
        graphAndEdgesSecond};

    return input;
}

void testGraph(vi pair)
{
    /* DEBUG */
    cerr << " Number of nodes per subgraph: " << pair.size() << endl
         << flush;
    /* DEBUG */

    cout << "test";
    for (size_t j = 0; j < pair.size(); j++)
    {
        /* DEBUG */
        // cerr << "At: " << pair.at(j) << endl << flush;
        /* DEBUG */

        cout << " ";
        cout << pair.at(j);
    }

    cout << endl
         << flush << endl;

    /* DEBUG */
    // cerr << pair.at(0) << endl << flush << endl;
    /* DEBUG */
}

void updateInfected(vi subgraph, vector<bool> &infected, vvi &toTest)
{
    string result;
    cin >> result;
    bool boolResult;
    if (result == "true")
    {
        /* DEBUG */
        // cerr << index << endl;
        /* DEBUG */

        if (subgraph.size() > 1)
        {
            toTest.push_back(subgraph);
        }

        boolResult = true;
    }
    else
    {
        boolResult = false;
    }

    for (int j = 0; j < subgraph.size(); j++)
    {
        infected[subgraph.at(j)] = boolResult;
    }
}

void manualTest(vvi toTest, vector<bool> &infected)
{
    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            cout << "test " << toTest.at(i).at(j) << endl
                 << flush << endl;
        }
    }

    for (size_t i = 0; i < toTest.size(); i++)
    {
        for (size_t j = 0; j < toTest.at(i).size(); j++)
        {
            string result;
            cin >> result;
            bool boolResult;
            if (result == "true")
            {
                boolResult = true;
            }
            else
            {
                boolResult = false;
            }

            infected[toTest.at(i).at(j)] = boolResult;
        }
    }
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

void recursiveTest(AdjacencyMatrix &adjMatrix, vvi inputPairs, vector<bool> &infected, float threshold, int minGroups)
{
    cerr << "recursive test: " << inputPairs.size() << ", threshold: " << threshold << endl;
    unvisitPairsInMatrix(inputPairs, adjMatrix);
    vector<vector<int>> pairs;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, threshold, minGroups);
    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testGraph(pairs.at(i));
        updateInfected(pairs.at(i), infected, toTest);
    }

    if (toTest.size() > 0)
    {
        vvi recursiveTestPairs;
        vvi manualTestPairs;

        for (int i = 0; i < toTest.size(); i++)
        {
            if (toTest.at(i).size() > recursionThreshold)
            {
                recursiveTestPairs.push_back(toTest.at(i));
            }
            else
            {
                manualTestPairs.push_back(toTest.at(i));
            }
        }

        float newThreshold = threshold - thresholdRecursionStep;
        cerr << "newThreshold: " << newThreshold << endl;
        minGroups = minGroups * 2;
        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold, minGroups);
        manualTest(manualTestPairs, infected);
    }
}

void runTestCase(AdjacencyMatrix &adjMatrix, vector<bool> &infected)
{
    vector<vector<int>> pairs;
    int minGroups = 2;
    pairs = adjMatrix.findDenseSubGraph(0, pairs, baseThreshold, minGroups);

    vvi toTest;

    for (size_t i = 0; i < pairs.size(); i++)
    {
        testGraph(pairs.at(i));
        updateInfected(pairs.at(i), infected, toTest);
    }

    if (toTest.size() > 0)
    {
        vvi recursiveTestPairs;
        vvi manualTestPairs;

        for (int i = 0; i < toTest.size(); i++)
        {
            if (toTest.at(i).size() > recursionThreshold)
            {
                recursiveTestPairs.push_back(toTest.at(i));
            }
            else
            {
                manualTestPairs.push_back(toTest.at(i));
            }
        }

        float newThreshold = baseThreshold - thresholdRecursionStep;

        recursiveTest(adjMatrix, recursiveTestPairs, infected, newThreshold, (minGroups*2));
        manualTest(manualTestPairs, infected);
    }
}

void answerTestCase(vector<bool> &infected, int nNodes)
{
    cout << "answer ";
    bool first = true;
    for (int i = 0; i < nNodes; i++)
    {
        if (infected[i])
        {
            if (not first)
            {
                cout << " ";
            }
            cout << i;
            first = false;
        }
    }
    cout << endl
         << flush;
}

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

void handleCredentials()
{
    fstream credFile("../credentials");
    string username, password;
    credFile >> username >> password;
    // and send them to the server
    cout << username << endl
         << flush;
    cout << password << endl
         << flush;
}

void runTestCases(int numCase, int &numCorrect)
{
    for (int testcase = 1; testcase <= numCase; testcase++)
    {
        Input input = parseInput();

        vector<bool> infected(input.nNodes, false);

        AdjacencyMatrix adjMatrix = createAdjacencyMatrix(input);

        runTestCase(adjMatrix, infected);

        answerTestCase(infected, input.nNodes);

        // read in the result and show it to the user
        string result;
        cin >> result;
        cerr << "Test case " << testcase << ": " << result << endl;
        if (result == "success")
            numCorrect++;
    }
}

int main()
{
    handleCredentials();

    // read the number of cases and report it to the user
    int numCase;
    cin >> numCase;
    // using cerr prints it to console instead of to the server
    cerr << "number of test cases: " << numCase << endl
         << flush << endl;
    int numCorrect = 0;

    runTestCases(numCase, numCorrect);

    // tell the user how many testcases were correct
    cerr << numCorrect << "/" << numCase << " correct" << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef int64_t ll;

double treshold = 1;

struct Point
{
    float diff;
    int source;
    int dest;
    bool visited = false;
};

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
            vector<int> accepted;
            accepted.push_back(currentIndex);
            pairs.push_back(accepted);
            return pairs;
        }
        if (visited[currentIndex])
        {
            cerr << "curr index: " << currentIndex << endl;
            int next = currentIndex + 1;
            return findDenseSubGraph(next, pairs);
        }
        visited[currentIndex] = true;

        vector<float> difference;

        for (size_t i = 0; i < n; i++)
        {
            if (i == currentIndex)
            {
                continue;
            }
            float diff = calcDifference(adj[currentIndex], adj[i]);
            difference.push_back(diff);
        }

        float average = accumulate(difference.begin(), difference.end(), 0.0) / n;

        // cerr << "average: " << average << endl;
        float acceptable = average;

        acceptable = average + (average * treshold);

        // cerr << "acceptable: " << acceptable << endl;
        vector<int> accepted;
        accepted.push_back(currentIndex);
        for (size_t i = 0; i < n; i++)
        {
            if (i == currentIndex)
                continue;
            if (difference[i] >= acceptable && visited[i] == false)
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
        float diff = (float)n;
        for (size_t i = 0; i < n; i++)
        {
            if (arr1[i] != arr2[i])
            {
                diff--;
            }
        }
        return diff / n;
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

// Parses the cin into the Input struct
Input parseInput()
{
    int nNodes, nEdges, nInfected;
    cin >> nNodes >> nEdges >> nInfected;
    long double infectionChance;
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

int main()
{
    fstream credFile("../credentials");
    string username, password;
    credFile >> username >> password;
    // and send them to the server
    cout << username << endl
         << flush;
    cout << password << endl
         << flush;

    // read the number of cases and report it to the user
    int numCase;
    cin >> numCase;
    // using cerr prints it to console instead of to the server
    cerr << "number of test cases: " << numCase << endl
         << flush << endl;
    int numCorrect = 0;
    // loop over all testcases
    for (int testcase = 1; testcase <= numCase; testcase++)
    {
        Input input = parseInput();

        AdjacencyMatrix adjMatrix(input.nNodes);
        for (size_t i = 0; i < input.edges.size(); i++)
        {
            int a, b;
            a = input.edges.at(i).first;
            b = input.edges.at(i).second;
            adjMatrix.addEdge(a, b);
        }
        // adjMatrix.dfs(0);
        // adjMatrix.bfs(0);
        vector<vector<int>> pairs;

        pairs = adjMatrix.findDenseSubGraph(0, pairs);
        // adjMatrix.display();

        // test everyone individually (basic solution, gets you no points)
        vector<bool> infected(input.nNodes, false);
        int counter = 0;

        for (size_t i = 0; i < pairs.size(); i++)
        {
            /* code */
            counter++;
            cerr << " Number of nodes per subgraph: " << pairs.at(i).size() << endl
                 << flush;
            for (size_t j = 0; j < pairs.at(i).size(); j++)
            {
                cerr << "At: " << pairs.at(i).at(j) << endl
                     << flush;
            }
            cout << "test " << pairs.at(i).at(0) << endl
                 << flush << endl;
            // cerr << pairs.at(i).at(0) << endl << flush << endl;
        }

        cerr << "Number of subgraphs: " << counter << endl
             << flush << endl;
        cerr << "Number of nodes: " << input.nNodes << endl
             << flush << endl;

        for (size_t i = 0; i < pairs.size(); i++)
        {
            string result;
            cin >> result;
            if (result == "true")
            {
                infected[i] = true;
            }
        }

        cout << "answer ";
        bool first = true;
        for (int i = 0; i < input.nNodes; i++)
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

        // for (int i = 0; i < input.nNodes; i++)
        // {
        //     // always flush after endline, otherwise it might not send it
        //     // immediately to the server but keep it in buffer.
        //     cout << "test " << i << endl
        //          << flush << endl;
        // }
        // read in results of those tests
        // note: separating sending/receiving means we don't have to wait
        // each test for the packet to travel to the server and back.
        // for (int i = 0; i < input.nNodes; i++)
        // {
        //     string result;
        //     cin >> result;
        //     if (result == "true")
        //     {
        //         infected[i] = true;
        //     }
        // }

        // hand in the answer
        // cout << "answer ";
        // bool first = true;
        // for (int i = 0; i < input.nNodes; i++)
        // {
        //     if (infected[i])
        //     {
        //         if (not first)
        //         {
        //             cout << " ";
        //         }
        //         cout << i;
        //         first = false;
        //     }
        // }
        // cout << endl
        //      << flush;

        // read in the result and show it to the user
        string result;
        cin >> result;
        cerr << "Test case " << testcase << ": " << result << endl;
        if (result == "success")
            numCorrect++;
    }

    // tell the user how many testcases were correct
    cerr << numCorrect << "/" << numCase << " correct" << endl;

    return 0;
}

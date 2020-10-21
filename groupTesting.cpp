#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef int64_t ll;

struct Input {
	int nNodes;
	int nEdges;
	int nInfected;
	long double infectionChance;
	int minInfected;
	int maxInfected;
	vvi graph;
	vii edges;
};

// Creates a graph adjecency list and a edge pairing list
pair<vvi, vii> parseGraphAndEdges(int nEdges) {
	vvi graph(nEdges);
	vii edges(nEdges);
	for (int i = 0; i < nEdges; i++) {
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
		graph[b].push_back(a);
		ii edge1 = { a, b };
		ii edge2 = { b, a };
		edges.push_back(edge1);
		edges.push_back(edge2);
	}

	pair<vvi, vii> graphAndEdges = { graph, edges };

	return graphAndEdges;
}

// Parses the cin into the Input struct
Input parseInput() {
	int nNodes, nEdges, nInfected;
	cin >> nNodes >> nEdges >> nInfected;
	long double infectionChance;
	cin >> infectionChance;
	int minInfected, maxInfected;
	cin >> minInfected >> maxInfected;
	
	pair<vvi, vii> graphAndEdges = parseGraphAndEdges(nEdges);
	vvi graph = graphAndEdges.first;
	vii edges = graphAndEdges.second;

	Input input = {
		nNodes,
		nEdges,
		nInfected,
		infectionChance,
		minInfected,
		maxInfected,
		graph,
		edges
	};

	return input;
}

int main() {
	fstream credFile("../credentials");
	string username, password;
	credFile >>username >> password;
	// and send them to the server
	cout << username << endl << flush;
	cout << password << endl << flush;

	// read the number of cases and report it to the user
	int numCase;
	cin >> numCase;
	// using cerr prints it to console instead of to the server
	cerr << "number of test cases: " << numCase << endl;
	int numCorrect = 0;
	// loop over all testcases
	for (int testcase = 1; testcase <= numCase; testcase++) {
		Input input = parseInput();

		// test everyone individually (basic solution, gets you no points)
		vector<bool> infected(input.nNodes,false);
		for (int i = 0; i < input.nNodes; i++) {
			// always flush after endline, otherwise it might not send it
			// immediately to the server but keep it in buffer.
			cout << "test " << i << endl << flush << endl;
		}
		// read in results of those tests
		// note: separating sending/receiving means we don't have to wait
		// each test for the packet to travel to the server and back.
		for (int i = 0; i < input.nNodes; i++) {
			string result;
			cin >> result;
			if (result == "true") {
				infected[i] = true;
			}
		}

		// hand in the answer
		cout << "answer ";
		bool first = true;
		for (int i = 0; i < input.nNodes; i++) {
			if (infected[i]) {
				if (not first) {
					cout << " ";
				}
				cout << i;
				first = false;
			}
		}
		cout << endl << flush;

		// read in the result and show it to the user
		string result;
		cin >> result;
		cerr << "Test case " << testcase << ": " << result << endl;
		if (result == "success") numCorrect++;
	}

	// tell the user how many testcases were correct
	cerr << numCorrect << "/" << numCase << " correct" << endl;

	return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef int64_t ll;

typedef struct Node {
	int index;
	vector<Node> adjacentNodes;
	vector<Node> visitedNodes;
	vector<Node> toVisitNodes;
	struct Node* prev = NULL;
	bool visited = false;
	bool partOfSubgraph = false;
} Node;

struct graphDatastructures {
	vvi intGraph;
	vii edges;
	vector<Node> nodeGraph;
};

struct Input {
	int nNodes;
	int nEdges;
	int nInfected;
	long double infectionChance;
	int minInfected;
	int maxInfected;
	vector<Node> nodeGraph;
	vvi graph;
	vii edges;
};

Node& findOrInitializeNode(vector<Node> graph, int index) {
	for (Node node : graph) {
		if (node.index == index) {
			return node;
		}
	}
	
	vector<Node> adjacencyList;
	vector<Node> toVisitNodes;
	vector<Node> visitedNodes;
	Node node = {
		index,
		adjacencyList,
		toVisitNodes,
		visitedNodes
	};

	graph.push_back(node);

	return node;
}

// Creates a graph adjacency list and a edge pairing list
graphDatastructures parseGraph(int nEdges, int nNodes) {
	vvi intGraph(nEdges);
	vii edges(nEdges);
	vector<Node> nodeGraph(nNodes);
	for (int i = 0; i < nEdges; i++) {
		int a, b;
		cin >> a >> b;
		// create node graph
		Node nodeA = findOrInitializeNode(nodeGraph, a); 
		Node nodeB = findOrInitializeNode(nodeGraph, b);
		nodeA.adjacentNodes.push_back(nodeB);
		nodeB.adjacentNodes.push_back(nodeA);
		cerr << "size:" << nodeA.adjacentNodes.size() << endl;

		// create int graph
		intGraph[a].push_back(b);
		intGraph[b].push_back(a);

		// create edges
		ii edge1 = { a, b };
		ii edge2 = { b, a };
		edges.push_back(edge1);
		edges.push_back(edge2);
	}

	graphDatastructures datastructures = { 
		intGraph, 
		edges,
		nodeGraph
	};

	return datastructures;
}

// Parses the cin into the Input struct
Input parseInput() {
	int nNodes, nEdges, nInfected;
	cin >> nNodes >> nEdges >> nInfected;
	long double infectionChance;
	cin >> infectionChance;
	int minInfected, maxInfected;
	cin >> minInfected >> maxInfected;
	
	graphDatastructures datastructures = parseGraph(nEdges, nNodes);
	vvi intGraph = datastructures.intGraph;
	vii edges = datastructures.edges;
	vector<Node> nodeGraph = datastructures.nodeGraph;

	Input input = {
		nNodes,
		nEdges,
		nInfected,
		infectionChance,
		minInfected,
		maxInfected,
		nodeGraph,
		intGraph,
		edges
	};

	return input;
}

void visitNode(Node node, vector<vector<Node>> subGraphs, Node* previous) {
	// Check if node has been visited.
	if (node.visited == true) {
		// If there are other nodes left to visit that are not part of an answer visit them.
		bool backtrack = true;
		for (Node toVisitNode : node.toVisitNodes) {
			if (!toVisitNode.partOfSubgraph) {
				backtrack = false;
				visitNode(node.toVisitNodes.front(), subGraphs, &node);
			}
		}

		// Otherwise backtrack further.
		if (backtrack == true) {
			// Stop if backtracking isn't possible.
			if (node.prev == NULL) {
				return;
			} else {
				visitNode(*node.prev, subGraphs, previous);
			}
		}
	}

	// Mark this node as visited.
	node.visited = true;
	node.prev = previous;

	// Sort nodes based on if they are visited already or not.
	for (Node adjacentNode : node.adjacentNodes) {
		if (node.visited == false) {
			node.toVisitNodes.push_back(adjacentNode);
		} else {
			node.visitedNodes.push_back(adjacentNode);
		}
	}

	// If no nodes are left to visit then create subgraph and start backtracking.
	if (node.toVisitNodes.empty()) {
		// Create subGraph
		vector<Node> subGraph;

		for (Node visitedNode : node.visitedNodes) {
			visitedNode.partOfSubgraph = true;
			subGraph.push_back(node);
		}

		node.partOfSubgraph = true;
		subGraph.push_back(node);

		// Add Subgraph
		subGraphs.push_back(subGraph);

		// Backtrack
		if (node.prev == NULL) {
			return;
		} else {
			visitNode(*node.prev, subGraphs, previous);
		}
	}

	// Continue visiting nodes
	Node next = node.toVisitNodes.front();
	visitNode(next, subGraphs, &node);
}

vector<vector<Node>> findDenseSubgraphs(vector<Node> graph) {
	vector<vector<Node>> subGraphs;

	Node node = graph.front();
	
	visitNode(node, subGraphs, NULL);

	return subGraphs;
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

		vector<vector<Node>> subGraphs = findDenseSubgraphs(input.nodeGraph);
		cerr << "subGraph size: "<< subGraphs.size() << endl;
		cerr << "nodeGraph size: " << input.nodeGraph.size() << endl;

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
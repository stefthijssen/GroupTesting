#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef int64_t ll;

int main() {
	fstream credFile("credentials");
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
		// read in testcase
		int numNode, numEdge, numInitInfected;
		cin >> numNode >> numEdge >> numInitInfected;
		long double infectionRate;
		cin >> infectionRate;
		int minInfected, maxInfected;
		cin >> minInfected >> maxInfected;
		vvi adjList(numNode);
		for (int edge = 0; edge < numEdge; edge++) {
			int from, to;
			cin >> from >> to;
			adjList[from].push_back(to);
			adjList[to].push_back(from);
		}

		// test everyone individually (basic solution, gets you no points)
		vector<bool> infected(numNode,false);
		for (int i = 0; i < numNode; i++) {
			// always flush after endline, otherwise it might not send it
			// immediately to the server but keep it in buffer.
			cout << "test " << i << endl << flush << endl;
		}
		// read in results of those tests
		// note: separating sending/receiving means we don't have to wait
		// each test for the packet to travel to the server and back.
		for (int i = 0; i < numNode; i++) {
			string result;
			cin >> result;
			if (result == "true") {
				infected[i] = true;
			}
		}

		// hand in the answer
		cout << "answer ";
		bool first = true;
		for (int i = 0; i < numNode; i++) {
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
#include <fstream>
#include <iostream>
using namespace std;

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
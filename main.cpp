#include <iostream>
#include <net/P2PNode.h>

using namespace std;

int main() {
    cout << "running" << endl;
    // CommandRunner runner;
    // runner.Run();

    P2PNode node(8892);

    node.run();

    return 1;
}
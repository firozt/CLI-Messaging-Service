#include <iostream>
#include <net/P2PNode.h>
#include <utils/TerminalInput.h>

using namespace std;

int main() {
    cout << "running" << endl;

    enableRawMode();
    // CommandRunner runner;
    // runner.Run();
    P2PNode node(8892);

    node.run();

    disableRawMode();
    return 1;
}
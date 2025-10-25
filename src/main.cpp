#include <iostream>
#include <net/P2PNode.h>
#include <utils/TerminalInput.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "running123" << endl;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    uint16_t port = static_cast<uint16_t>(std::stoi(argv[1]));


    // enableRawMode();
    // CommandRunner runner;
    // runner.Run();
    P2PNode node(port);
    node.run();

    disableRawMode();
    return 1;
}
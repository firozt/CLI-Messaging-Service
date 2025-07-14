//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef P2PNODE_H
#define P2PNODE_H
#include <net/BaseServer.h>


class P2PNode: public BaseServer {
private:
    void handleClient(int conn_fd) override;

    void receiveListener(int conn_fd);

    void sendListener(int conn_fd);

public:
    P2PNode(uint16_t port);
    std::string currentInput;
    std::mutex inputMutex;
};



#endif //P2PNODE_H

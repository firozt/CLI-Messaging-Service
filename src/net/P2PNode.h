//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef P2PNODE_H
#define P2PNODE_H
#include <net/BaseServer.h>


class P2PNode: public BaseServer {
private:
    void handleClient(int conn_fd) override;
    void recieveListener(int conn_fd);
    void sendListener(int conn_fd);
public:
    P2PNode(uint16_t port);
};



#endif //P2PNODE_H

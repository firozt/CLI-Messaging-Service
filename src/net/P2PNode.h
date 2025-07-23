//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef P2PNODE_H
#define P2PNODE_H

#include "BaseServerUDP.h"


class P2PNode: public BaseServerUDP {
private:

public:
    P2PNode(uint16_t port);
    void run();
};



#endif //P2PNODE_H

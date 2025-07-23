//
// Created by Ramiz Abdulla on 14/07/2025.
//

#include "P2PNode.h"
#include <iostream>
#include <unistd.h> 
#include <thread>
#include <sys/socket.h>

#include <utils/TerminalInput.h>



P2PNode::P2PNode(uint16_t port): BaseServerUDP(port) {
}

void P2PNode::run() {
    // call super
    BaseServerUDP::run();
}

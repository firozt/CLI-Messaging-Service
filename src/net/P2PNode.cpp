//
// Created by Ramiz Abdulla on 14/07/2025.
//

#include "P2PNode.h"
#include <iostream>
#include <unistd.h>   // for read(), write(), close()
#include <cstring>    // for strlen()
#include <sys/socket.h>

P2PNode::P2PNode(uint16_t port): BaseServer(port,99) { }


// when a peer connects to this node it will spawn a new thread and call this method accompanied by the connection id
void P2PNode::handleClient(int conn_fd) {



    close(conn_fd); // cleanup
}

void P2PNode::sendListener(int conn_fd) {
    while (true) {
        std::cout << "You: ";
        std::string reply;
        std::getline(std::cin, reply);

        if (reply == "/quit") {
            std::cout << "Closing connection.\n";
            break;
        }

        // 3️⃣ Send the reply
        ssize_t bytes_sent = send(conn_fd, reply.c_str(), reply.length(), 0);
        if (bytes_sent < 0) {
            perror("send failed");
            break;
        }
    }
}


void P2PNode::recieveListener(int conn_fd) {
    char buffer[1024];
    while (true) {
        // 1️⃣ Receive message from peer
        ssize_t bytes_received = recv(conn_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "Peer disconnected.\n";
            } else {
                perror("recv failed");
            }
            break; // exit loop & thread
        }

        // Null-terminate & print
        buffer[bytes_received] = '\0';
        std::cout << "Peer: " << buffer << std::endl;
    }
}


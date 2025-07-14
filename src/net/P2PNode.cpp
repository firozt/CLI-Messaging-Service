//
// Created by Ramiz Abdulla on 14/07/2025.
//

#include "P2PNode.h"
#include <iostream>
#include <unistd.h> 
#include <thread>
#include <sys/socket.h>

#include <utils/TerminalInput.h>


P2PNode::P2PNode(uint16_t port): BaseServer(port,99) { }

// when a peer connects to this node it will spawn a new thread and call this method accompanied by the connection id
void P2PNode::handleClient(int conn_fd) {
    std::thread t1([this,conn_fd]() {
        // use corresponding handler
        this->receiveListener(conn_fd);
    });
    std::thread t2([this,conn_fd]() {
        // use corresponding handler
        this->sendListener(conn_fd);
    });

    // wait for t1 and t2 to end (should happens same time roughly)
    t1.join();
    t2.join();

    close(conn_fd); // cleanup
}

void P2PNode::sendListener(int conn_fd) {
    while (true) {
        std::lock_guard<std::mutex> lock(inputMutex);

        int c = readUserInputChar(currentInput);
        if (c == -1) continue;  // error or EOF, skip
        if (c == 0) continue;   // backspace handled, no char added

        if (c == '\n') {
            if (currentInput == "/quit") {
                std::cout << "\nClosing connection.\n";
                break;
            }
            std::string msg = currentInput + "\n";
            ssize_t bytes_sent = send(conn_fd, msg.c_str(), msg.size(), 0);
            if (bytes_sent < 0) {
                perror("send failed");
                break;
            }
            currentInput.clear();
            std::cout << "\r\033[KYou: " << std::flush;  // Clear line and print prompt
        }
        else {
            currentInput.push_back(static_cast<char>(c));
        }
    }
}


void P2PNode::receiveListener(int conn_fd) {
    char buffer[1024];
    while (true) {
        ssize_t bytes_received = recv(conn_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "\nPeer disconnected.\n";
            } else {
                perror("recv failed");
            }
            break;
        }

        buffer[bytes_received] = '\0';

        // Clear current line (user input)
        std::cout << "\r\033[K";

        // Print the incoming message
        std::cout << "Peer-" << conn_fd << ": " << buffer;

        // Reprint prompt + current input line
        std::cout << "You: " << currentInput;
        std::cout.flush();
    }
}


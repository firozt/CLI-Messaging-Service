//
// Created by Ramiz Abdulla on 23/07/2025.
//

#include <cinttypes>
#include <iostream>
#include <thread>
#include <net/BaseServerUDP.h>

#include "utils/TerminalInput.h"

BaseServerUDP::BaseServerUDP(uint16_t port): port(port) {}


int BaseServerUDP::create_socket(sockaddr_in &address, int addr_len) {
    //  create socket
    // ipv4 protocol (af_inet), via tcp (sock_stream)
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    // socket_fd should be > 0 to be valid else error
    if (socket_fd < 0) {
        perror("Socket creation failed\n");
        return -1;
    }

    //  bind socket with address and port
    int bind_status = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address), static_cast<socklen_t>(addr_len));

    if (bind_status < 0) {
        printf("Unable to bind socket for port %d\n",port);
        return -1;
    }
    return socket_fd;
}


void BaseServerUDP::run() {

    // create address object
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    int addr_len = sizeof(address);
    // creates a socket
    int socket_fd = create_socket(address, addr_len);
    // listening thread
    std::thread t([this, socket_fd, &address, addr_len]() {
        this->listen_loop(socket_fd,address,addr_len);
    });


    // send listening thread
    std::thread t2([this, socket_fd, &address, addr_len]() {
            this->send_loop(socket_fd,address,addr_len);
    });


    t.join();
}

int BaseServerUDP::listen_loop(int socket_fd, sockaddr_in &address, int addr_len) {
    while (true) {
        char buffer[1024];
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        ssize_t received_bytes = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0,reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (received_bytes < 0) {
            perror("recvfrom failed");
            continue;
        }
        buffer[received_bytes] = '\0';  // null-terminate if treating as string

        // Clear current line (user input)
        std::cout << "\r\033[K";

        // Print the incoming message
        std::cout << "Peer" << socket_fd << ": " << buffer;

        // Reprint prompt + current input line
        std::cout << "You: " << currentInput;
        std::cout.flush();
    }
}

void BaseServerUDP::send_loop(int socket_fd, sockaddr_in &address, int addr_len) {
    while (true) {
        std::lock_guard<std::mutex> lock(this->inputMutex);

        int c = readUserInputChar(currentInput);
        if (c == -1) continue;  // error or EOF, skip
        if (c == 0) continue;   // backspace handled, no char added

        if (c == '\n') {
            if (currentInput == "/quit") {
                std::cout << "\nClosing connection.\n";
                break;
            }
            std::string msg = currentInput + "\n";
            ssize_t bytes_sent = send(socket_fd, msg.c_str(), msg.size(), 0);
            if (bytes_sent < 0) {
                perror("send failed");
                break;
            }
            currentInput.clear();
            std::cout << "\r\033" << std::flush;  // Clear line and print prompt
        }
        else {
            currentInput.push_back(static_cast<char>(c));
        }
    }
}



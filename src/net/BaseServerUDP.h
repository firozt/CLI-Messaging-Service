//
// Created by Ramiz Abdulla on 23/07/2025.
//

#include <netinet/in.h>
#include <string>
#include <mutex>

#ifndef BASESERVERUDP_H
#define BASESERVERUDP_H

#endif //BASESERVERUDP_H


class BaseServerUDP {
public:
    uint16_t port;
    std::string currentInput;
    std::mutex inputMutex;
    void run();
    BaseServerUDP(uint16_t port);
private:
    int create_socket(sockaddr_in& address, int addr_len);
    int listen_loop(int socket_fd, sockaddr_in& address, int addr_len);
    void send_loop(int socket_fd, sockaddr_in& address, int addr_len);
};
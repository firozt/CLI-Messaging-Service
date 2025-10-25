//
// Created by Ramiz Abdulla on 14/07/2025.
//

#include "P2PNode.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>




P2PNode::P2PNode(uint16_t port) {
    this->port = port;
}

// creates a UDP socket and binds it, returns socket_fd or -1 on error
int P2PNode::create_socket(sockaddr_in &address, int addr_len) {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed\n");
        return -1;
    }

    int bind_status = bind(socket_fd, (struct sockaddr *)&address, (socklen_t)addr_len);
    if (bind_status < 0) {
        printf("Unable to bind socket for port %d\n", port);
        return -1;
    }

    // get the local IP assigned
    sockaddr_in local_addr{};
    socklen_t local_len = sizeof(local_addr);
    if (getsockname(socket_fd, (struct sockaddr *)&local_addr, &local_len) == 0) {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &local_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
        std::printf("Listening on IP: %s, Port: %d\n", ip_str, ntohs(local_addr.sin_port));
    } else {
        perror("getsockname failed");
    }

    return socket_fd;
}


void P2PNode::run() {

    // create address object
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    int addr_len = sizeof(address);
    // creates a socket
    int socket_fd = create_socket(address, addr_len);

    if (socket_fd == -1) {
        std::printf("Socket creation failed");
        return;
    }

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

int P2PNode::listen_loop(int socket_fd, sockaddr_in &address, int addr_len) {
    char msg_buff[1024];

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        ssize_t msg_size = recvfrom(socket_fd,
                                    msg_buff,
                                    sizeof(msg_buff) - 1,
                                    0,
                                    (sockaddr*)&client_addr,
                                    &client_len);

        if (msg_size < 0) {
            perror("recvfrom failed");
            continue;  // keep listening
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
        printf("%s:%hu -> %s\n", ip_str, ntohs(client_addr.sin_port), msg_buff);

        // reset buffer
        msg_buff[msg_size] = '\0';  // null terminate

    }

    return 0;
}


int P2PNode::send_loop(int socket_fd, sockaddr_in &address, int addr_len) {
    std::string cur_input;

    while (true) {
        std::getline(std::cin, cur_input);

        sockaddr_in dest_addr{};
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(get_port_input(cur_input));
        inet_pton(AF_INET, get_ip_input(cur_input).c_str(), &dest_addr.sin_addr);  // IP

        std::string msg = extract_msg(cur_input);

        ssize_t sent = sendto(socket_fd,
                              msg.c_str(),
                              cur_input.size(),
                              0,
                              (struct sockaddr*)&dest_addr,
                              sizeof(dest_addr));
        if (sent < 0) {
            perror("sendto failed");
        }
    }
}

std::string P2PNode::get_ip_input(std::string input) {
    size_t pos = input.find(':');
    return (pos != std::string::npos) ? input.substr(0, pos) : input;
}

uint16_t P2PNode::get_port_input(std::string input) {
    size_t colon_pos = input.find(':');
    if (colon_pos == std::string::npos) {
        throw std::invalid_argument("No colon found in input");
    }

    size_t space_pos = input.find(' ', colon_pos);  // find first space after colon

    // length of the port substring
    size_t length = (space_pos == std::string::npos) ? std::string::npos : space_pos - colon_pos - 1;

    std::string port_str = input.substr(colon_pos + 1, length);  // skip colon

    int port = std::stoi(port_str);  // convert string to int
    if (port < 0 || port > 65535) {
        throw std::out_of_range("Port out of range");
    }

    return static_cast<uint16_t>(port);
}

std::string P2PNode::extract_msg(std::string input) {
    size_t pos = input.find(' ');
    return (pos != std::string::npos) ? input.substr(pos+1) : "error";

}
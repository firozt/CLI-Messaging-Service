//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef P2PNODE_H
#define P2PNODE_H
#include <cstdint>
#include <string>
#include <netinet/in.h>


class P2PNode {
private:

public:
    P2PNode(uint16_t port);
    std::string cur_input;

    void run();
    uint16_t port;
    std::string get_ip_input(std::string input);
    uint16_t get_port_input(std::string input);
    std::string extract_msg(std::string input);


private:
    int create_socket(sockaddr_in& address, int addr_len);
    int listen_loop(int socket_fd, sockaddr_in &address, int addr_len);
    int send_loop(int socket_fd, sockaddr_in &address, int addr_len);

};



#endif //P2PNODE_H

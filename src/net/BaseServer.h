//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef BASESERVER_H
#define BASESERVER_H
#include <utils/ConnectionManager.h>
#include <netinet/in.h>


/**
 * Abstract
 * @class BaseServer
 * @brief Basic functionalities of a TCP IPv4 server, listens to a port, manages connections
 * actions of connected clients is handled by the virtual method handleClient, that must be
 * overwritten in child classes
 */


class BaseServer {
public:
    // properties
    const uint16_t port;
    ConnectionManager connections;
    // constructor
    BaseServer(uint16_t port, int max_connections);

    ConnectionManager* get_connections();
    // methods
    void run();
private:
    int create_socket(sockaddr_in& address, int addr_len);

    // all child classes must override this
    virtual void handleClient(int conn_fd) = 0;


};



#endif //BASESERVER_H

//
// Created by Ramiz Abdulla on 14/07/2025.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <strings.h>
#include <net/BaseServer.h>


BaseServer::BaseServer(uint16_t port, int max_connections)
: port(port), connections(max_connections) {};


// creates a IPv4 TCP socket, binds it with the address config and listens to it
// returns the socket_fd
int BaseServer::create_socket(sockaddr_in& address, int addr_len) {
    //  create socket
    // ipv4 protocol (af_inet), via tcp (sock_stream)
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
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
    //  listen to the binded socket at client port
    // takes socket field descriptor and a max len of the queue
    int listen_status = listen(socket_fd, 10);

    if (listen_status < 0) {
        perror("Unable to create a listener for socket");
        return -1;
    }
    return socket_fd;
}



// returns a pointer to the current objects connection mannager object
ConnectionManager* BaseServer::get_connections() {
    return &connections;
}

void BaseServer::run(){
    // creates a listening socket

    // create address object
    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    int addr_len = sizeof(address);

    // create socket and prepares for listening
    int socket_fd = create_socket(address, addr_len);
    if (socket_fd < 0) {
        perror("Could not create socket");
        return;
    }

    printf("Server listening on port %d awaiting connections.\n", port);
    // accept connection
    // original thread will wait for connections, once one is requested new thread
    // is spawned and will handle broadcasted messages from source thread
    // number of destination clients will be handled by the destination client list

    while(true) {
        int conn_fd = accept(socket_fd, reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addr_len));
        if (conn_fd < 0) {
            perror("Failed to accept a source client\n");
            continue;
        }

        // attempts to add the connection
        // -1 indicates infinite number of connections
        if (connections.size() >= connections.get_max_connections() and connections.get_max_connections() != -1) {
            // reject this connection, send attempted client an error
            const char* err_msg = "Error: Source client already connected.\n";
            send(conn_fd, err_msg, strlen(err_msg), 0);
            close(conn_fd);
            continue;
        }

        // accept this connection
        connections.add(conn_fd);
        std::thread t([this,conn_fd]() {
            // use corresponding handler
                this->handleClient(conn_fd);
            // cleanup socket from connection list
            connections.remove(conn_fd);
            close(conn_fd);
        });
        t.detach();
    }
    close(socket_fd);
}

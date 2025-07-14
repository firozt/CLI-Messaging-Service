//
// Created by Ramiz Abdulla on 14/07/2025.
//


/**
 * @class ConnectionManager
 * @brief Thread safe way of managing list of connected clients
 * controls max number of clients
 */

#include "ConnectionManager.h"
ConnectionManager::ConnectionManager(int max_connections) : max_connections(max_connections) {}

int ConnectionManager::get_max_connections(){
    return max_connections;
}

bool ConnectionManager::add(int fd) {
    if (this->size() >= this->max_connections) {
        return false;
    }
    std::lock_guard<std::mutex> lock(mutex);
    clients.insert(fd);
    return true;
}

void ConnectionManager::remove(int fd) {
    std::lock_guard<std::mutex> lock(mutex);
    clients.erase(fd);
}

std::set<int> ConnectionManager::get_all() {
    std::lock_guard<std::mutex> lock(mutex);
    return clients;
}

size_t ConnectionManager::size() {
    std::lock_guard<std::mutex> lock(mutex);
    return clients.size();
}

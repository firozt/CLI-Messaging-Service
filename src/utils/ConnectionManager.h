//
// Created by Ramiz Abdulla on 14/07/2025.
//

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
#include <mutex>
#include <set>


class ConnectionManager {
private:
    std::mutex mutex;
    std::set<int> clients;
    int max_connections = 0;
public:
    ConnectionManager(int max_connections);

    int get_max_connections();

    bool add(int fd);

    void remove(int fd);

    std::set<int> get_all();

    size_t size();

};



#endif //CONNECTIONMANAGER_H

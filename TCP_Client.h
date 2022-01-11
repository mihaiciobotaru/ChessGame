#ifndef SFMLAPP_TCP_CLIENT_H
#define SFMLAPP_TCP_CLIENT_H

#include <iostream>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>


#define BUFFLEN 1024

struct package{
    int type;
    std::vector<int> data;
};

class TCP_client{
private:
    bool error;
    bool isConn;
    bool turn;
    char* server_ip;
    int PORT;
    int client_socket;
    struct sockaddr_in server_address{};
public:
    TCP_client();
    ~TCP_client();

    void send_board(std::vector<std::vector<int>> matrix);
    void send_exit();
    void retrive_package(package* info);
    bool getTurn();
    bool getConn();
};

#endif //SFMLAPP_TCP_CLIENT_H

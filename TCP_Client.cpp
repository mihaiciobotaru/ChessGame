#include "TCP_Client.h"

TCP_client::TCP_client() {
    isConn = false;
    error= false;
    server_ip = "192.168.100.14";
    PORT = 8080;

    client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket < 0){
        perror("[-]Could not create client socket!\n");
        error = true;
    }else std::cout<<"[+]Client socket created successfully!\n";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, server_ip, &server_address.sin_addr)<=0){
        perror("[-]Invalid address!\n");
        error = true;
    }
    do {
        if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            perror("[-]Could not connect to host!\n");
            error = true;
        } else{
            std::cout << "[+]Successfully connected to host!\n";
            error = false;
        }
    }while(error);
}

TCP_client::~TCP_client() {
    shutdown(client_socket,SHUT_RDWR);
}

void TCP_client::send_board(std::vector<std::vector<int>> matrix){
    turn = false;
    int board[64];
    for(int i = 0;i<8;++i)
        for(int j = 0;j<8;++j)
        {
            board[i*8+j]=matrix[i][j];
        }
    int bytes_sent = send(client_socket,board, sizeof(board),0);
    if(bytes_sent < 0){
        perror("[-]Could not send!\n");
    }
}

void TCP_client::send_exit() {
    int exit = 0;
    int bytes_sent = send(client_socket,&exit, sizeof(exit),0);
    if(bytes_sent < 0){
        perror("[-]Could not send!\n");
    }
    isConn = false;
    close(client_socket);
}

void TCP_client::retrive_package(package* info) {
    std::vector<int> processed_package;
    int package[64] = {};
    int bytes_received;
    bytes_received = recv(client_socket, package , sizeof(package), 0);
    if(bytes_received < 0){
        perror("[-]Could not receive.\n");
    }
    else if(bytes_received == 0){
        std::cout<<"[+]Server has disconnected.\n";
        processed_package.push_back(0);
        info->type = 0;
        info->data.push_back(0);
    }else if(bytes_received == 4){
        if(package[0] == 0){
            std::cout<<"[+]Shutting down request accepted.\n";
            info->type = 0;
            info->data.push_back(0);
        }else {
            info->type = 1;
            info->data.push_back(package[0]-1);
            turn = info->data[0];
            isConn = true;
        }
    }else {
        turn = true;
        info->type = 2;
        for(int i = 0;i<64;++i){
            info->data.push_back(package[i]);
        }
    }
}

bool TCP_client::getTurn() {
    return turn;
}

bool TCP_client::getConn() {
    return isConn;
}
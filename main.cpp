#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>

using namespace std;

#define PORT 8050
#define BUFFLEN 1024

int main(){

    /*
     * 1. create socket
     * 2. connect to host
     * 3. send information
     * 4. receive information
     * 5. goto 3 until stop command
     */

    /// Creating socket ///

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0){
        perror("[-]Could not create client socket!\n");
        return 1;
    }else cout<<"[+]Client socket created successfully!\n";

    struct sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    /// Connecting to host ///

    // Convert IPv4 and IPv6 addresses from text to binary form

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0){
        perror("[-]Invalid address!\n");
        return 1;
    }

    if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))<0){
        perror("[-]Could not connect to host!\n");
        return 1;
    }else cout<<"[+]Cuccessfully connected to host!\n";

    /// Communication with server ///

    char buffer[BUFFLEN];

    for(;;){
        /// sending

        string response = "Salut\n";
        int bytes_sent = send(client_socket,response.c_str(), response.length(),0);
        if(bytes_sent < 0){
            perror("[-]Could not send!\n");
            return 1;
        }
    }

    cout<<"[*]Shutting down host socket!\n";
    shutdown(client_socket,SHUT_RDWR);
    return 0;
}
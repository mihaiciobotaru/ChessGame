#include <iostream>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>

using namespace std;

#define BUFFLEN 1024


void communication(int client_socket){
  /// Communication with client ///

    char buffer[BUFFLEN];
    int bytes_received, bytes_sent;
    string message;
    for(;;){

        /// sending

        getline(cin,message);
        int bytes_sent = send(client_socket,message.c_str(), message.length(),0);
        if(message==":exit"){
            break;
        }
        if(bytes_sent < 0){
            perror("[-]Could not send!\n");
        }

        /// receiving

        memset(buffer, 0 , BUFFLEN);
        bytes_received = recv(client_socket, buffer , BUFFLEN - 1, 0);

        if(bytes_received < 0){
            perror("[-]Could not receive.\n");
        }
        if(bytes_received == 0){
            cout<<"[+]Server has disconnected.\n";
            break;
        }
        if(buffer[bytes_received-1]=='\n')buffer[bytes_received-1]=0;
        if(strcmp(":exit",buffer)==0){
            cout<<"[+]Shutting down request accepted.\n";
            break;
        }
        cout<<"[+]Server message : \""<<buffer<<"\"\n";

    }

}

int main(int argc, char *argv[]){

    //for the server, we only need to specify a port number
    if(argc != 3)
    {
        cerr << "[-]Invalid usage. Provide ip and port.\n" << endl;
        exit(0);
    }
    //grab the port number
    auto server_ip = argv[1];
    int PORT = atoi(argv[2]);

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

    if(inet_pton(AF_INET, server_ip, &server_address.sin_addr)<=0){
        perror("[-]Invalid address!\n");
        return 1;
    }

    if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))<0){
        perror("[-]Could not connect to host!\n");
        return 1;
    }else cout<<"[+]Successfully connected to host!\n";

    /// Communication with server ///

    communication(client_socket);

    cout<<"[+]Shutting down client socket.\n";
    shutdown(client_socket,SHUT_RDWR);
    return 0;
}

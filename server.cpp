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

bool exit_signal = false;

void communication(int client_socket){
  /// Communication with client ///

    char buffer[BUFFLEN];
    int bytes_received, bytes_sent;
    string message;
    for(;;){

        /// receiving

        memset(buffer, 0 , BUFFLEN);
        bytes_received = recv(client_socket, buffer , BUFFLEN - 1, 0);

        if(bytes_received < 0){
            perror("[-]Could not receive.\n");
        }
        if(bytes_received == 0){
            cout<<"[+]Client has disconnected.\n";
            break;
        }
        if(buffer[bytes_received-1]=='\n')buffer[bytes_received-1]=0;
        if(strcmp(":exit",buffer)==0){
            cout<<"[+]Shutting down request accepted.\n";
            break;
        }
        cout<<"[+]Client message : \""<<buffer<<"\"\n";

        /// sending

        getline(cin,message);
        int bytes_sent = send(client_socket,message.c_str(), message.length(),0);
        if(message==":exit"){
            exit_signal = true;
            break;
        }
        if(bytes_sent < 0){
            perror("[-]Could not send!\n");
        }
    }
}

int main(int argc, char *argv[]){

    //for the server, we only need to specify a port number
    if(argc != 2)
    {
        cerr << "[-]Invalid usage. Provide a port.\n" << endl;
        exit(0);
    }
    //grab the port number
    int PORT = atoi(argv[1]);

    /*
     * 1. create socket
     * 2. bind
     * 3. listen for connections
     * 4. accept connection
     * 5. recive information
     * 6. send information
     * 7. goto 5 until stop command
     */

    /// Creating socket ///

    int host_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(host_socket < 0){
        perror("[-]Could not create host socket.\n");
        return 1;
    }else cout<<"[+]Host socket created successfully.\n";

    /// Bind to port ///

    struct sockaddr_in address{};
    memset(&address, 0 ,sizeof (address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);


    if(bind(host_socket, (struct sockaddr *)&address, sizeof(address)) < 0 ){
        perror("[-]Could not bind.\n");
        return 1;
    }else cout<<"[+]Binded successfully.\n";

    /// Listening for connections ///

    for(;;){
            
        if(listen(host_socket,1)){
            perror("[-]Could not listen.\n");
            return 1;
        }else cout<<"[+]Waiting for connection.\n";

        /// Accepting connections ///

        struct sockaddr_in client_address{};
        memset(&client_address, 0 ,sizeof (address));
        socklen_t client_address_length = sizeof(address);

        int client_socket = accept(host_socket,(struct sockaddr *)&client_address , &client_address_length);
        int client_port = ntohs(client_address.sin_port);
        string client_ip = inet_ntoa(client_address.sin_addr);
        cout<<"[+]Accepted client with ip : "<<client_ip<<" and with port : "<<client_port<<" .\n";

        communication(client_socket);

        cout<<"[+]Shutting down client socket.\n";
        shutdown(client_socket,SHUT_RDWR);

        if(exit_signal == true)break;
    }
    cout<<"[+]Shutting down host socket.\n";
    shutdown(host_socket,SHUT_RDWR);

    return 0;
}

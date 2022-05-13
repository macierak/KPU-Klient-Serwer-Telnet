#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 5000000
#define DEFAULT_PORT "60000"
WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;
char sendbuf[100], clientPort[5];
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
int initialize(), connectToServer();
int __cdecl main(int argc, char **argv) {
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    initialize(argv[1], DEFAULT_PORT);


    
    initialize(argv[1], clientPort);
    // Attempt to connect to an address until one succeeds
    

    
    while(1){
        // Send an initial buffer
        memset(sendbuf, 0, 100);
        printf("%s:%s >", argv[1], clientPort);
        gets(sendbuf);
        if(!strcmp(sendbuf, "quit")){
        // shutdown the connection since no more data will be sent
            printf("--------------");
            break;
            if (iResult == SOCKET_ERROR) {
                printf("shutdown failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            break;
        }else{
            iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }   
            printf("Bytes Sent: %ld\n", iResult);
        }

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ){
            printf("Bytes received: %d\n", iResult);
            printf("Wiadomosc: %s \n", recvbuf);
        }
        else if ( iResult == 0 ){
            printf("Connection closed\n");
            break;
        }
        else
            printf("recv failed with error: %d\n", WSAGetLastError());


    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    
    return 0;
}
int initialize(char* adress, char* port){
// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(adress, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    connectToServer();
    if(!strcmp(port, DEFAULT_PORT)){
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if ( iResult > 0 ){
                strcpy(clientPort, recvbuf);
                printf("Port podprocesu: %s \n", clientPort);
                
            }
    }   
   
}
int connectToServer(){
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    
}
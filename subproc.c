#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "60010"

WSADATA wsaData;
int iResult, iSendResult, recvbuflen = DEFAULT_BUFLEN, port;
SOCKET ListenSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
struct addrinfo *result = NULL, hints;

char recvbuf[DEFAULT_BUFLEN], portString[5];

int initialSetup();

int __cdecl main(int _argc, char * _argv[]){
    sprintf(portString, "%d", _argv[1]);
    while(1){
        if(initialSetup(_argv[1])){
            return 1;
        } 
        else{
            printf("Klient czeka na polaczenie na porcie %s\n", _argv[1]);
        };
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        printf("Nowe polaczenie z %d", ClientSocket);

        // Receive until the peer shuts down the connection  
        do {
            memset(recvbuf, 0, recvbuflen);
            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            printf("\n %d \n",iResult);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                printf("Wiadomosc: %s \n", recvbuf);

                FILE *fp;
                char path[1035];

                fp = popen(recvbuf, "r");
                while (fgets(path, sizeof(path), fp) != NULL) {
                    printf("%s", path);
                }



                // Echo the buffer back to the sender
                iSendResult = send( ClientSocket, path, iResult, 0 );
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 1;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }


            else if (iResult == 0){
                printf("Connection closing...\n");
            }
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        // cleanup
        closesocket(ClientSocket);
        WSACleanup();
        }


        return 0;
}   
int initialSetup(char* port ){
// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}
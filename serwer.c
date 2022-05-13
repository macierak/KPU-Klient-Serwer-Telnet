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
#define DEFAULT_PORT 60000

WSADATA wsaData;
int iResult, iSendResult, recvbuflen = DEFAULT_BUFLEN, port = DEFAULT_PORT;
SOCKET ListenSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
struct addrinfo *result = NULL, hints;
LPPROCESS_INFORMATION info;
char recvbuf[DEFAULT_BUFLEN], portString[5], newPort[5];
char cmd[] = "./subproc.exe ";
LPPROCESS_INFORMATION pi;
STARTUPINFO si;

int initialSetup(); int waitingForConnection();

int __cdecl main(void){
    initialSetup();
    printf("Serwer czeka na polaczenie na porcie %s\n", portString);
    while(1){
        ZeroMemory( & pi, sizeof( pi ) );
        ZeroMemory( & si, sizeof( si ) );
        si.cb = sizeof( si );

        
        waitingForConnection();

    }        

    return 0;

}
int waitingForConnection(){
    // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        strcpy(cmd, "./subproc.exe ");
        port++;
        sprintf(portString, "%d", port);
        strcpy(newPort, portString);
        strcat(cmd, portString);
        if(CreateProcess(NULL, ( LPSTR ) cmd, NULL, NULL, FALSE, 0, NULL, NULL, & si, & pi ) ){
            printf("Stworzono proces \n");
            iSendResult = send( ClientSocket, newPort, iResult, 0 );
            iResult = shutdown(ClientSocket, SD_SEND);
            return 0;
        }else{
            return 1;
        }
}

int initialSetup(){
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
    sprintf(portString, "%d", port);
    iResult = getaddrinfo(NULL, portString, &hints, &result);
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
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define SERV_PORT 60000

    int connfd;
    struct sockaddr_in serwer_address;
    int n;
    char linia[100], liniaRcv[100], newPort[6];
    int socketfd; int err;
int main(int argc, char** argv){

    initialize(argv[1], SERV_PORT);
    if(connect(socketfd, &serwer_address, sizeof(serwer_address))< 0){
        fprintf(stderr, "\nBłąd przy połączeniu z serwerem");
        exit(1);
    }else{
        read(socketfd, liniaRcv, 100);
        strcpy(newPort, liniaRcv);
    }

    initialize(argv[1], atoi(newPort));
    if(connect(socketfd, &serwer_address, sizeof(serwer_address))< 0){
        fprintf(stderr, "\nBłąd przy połączeniu z serwerem");
        exit(1);
    }
    
    while(1){
        memset(liniaRcv, 0, 100);

        printf("%s:%s >", argv[1], newPort);
        gets(linia);
        n=strlen(linia);
        if(!strcmp(linia, "quit")){
            printf("------------'n");
            break;
        }
        write(socketfd, linia, n);
        n = read(socketfd, liniaRcv, 1000);
        printf(liniaRcv);

    }

    exit(0);
}

void initialize(char* adress, int port){  
    socketfd=socket(AF_INET, SOCK_STREAM, 0);  
    bzero(&serwer_address, sizeof(serwer_address));
    serwer_address.sin_family=AF_INET;
    serwer_address.sin_port=htons(port);
    err = inet_pton(AF_INET, adress, &serwer_address.sin_addr);   
}

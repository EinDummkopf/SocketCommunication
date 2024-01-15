#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUFSIZE 65536

char *CURR_MY_PATH_ROOT;

void error_handling(char *message);

int main(int argc, char **argv){
    CURR_MY_PATH_ROOT = getenv("PWD");

    int serv_sock;
    int clnt_sock;
    char message[BUFSIZE];
    int str_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);

    if(serv_sock == -1){
        error_handling("socket() error");
    }
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
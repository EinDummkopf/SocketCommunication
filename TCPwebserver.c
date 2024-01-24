#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#define BUFSIZE 65536
#define SEND_MESSAGE_BUFSIZE 1024

char *CURR_MY_PATH_ROOT;

void error_handling(char *message);
void request_handler(void *);
void GET_handler(char *, char *, char *, int);
void POST_handler(char *, char *, char *, int, char *);

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

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htonl(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind() error");
    }
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }
    while(1){
        clnt_addr_size=sizeof(clnt_addr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1){
    	    error_handling("accept() error");
            break;
        }  
        printf("Connection Request : %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        request_handler(&clnt_sock);
    }
    close(clnt_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}

void request_handler(void *arg){
    char msg[BUFSIZE];
    char *firstLine[3];

    int sd = *(int*)arg; // casting
    int rcvd =  recv(sd, msg, BUFSIZE-1, 0);
    if(rcvd<=0) error_handling("Error about recv()!!");
    printf("----------Request message from Client----------\n");
    printf("%s", msg);
    printf("\n----------------------------------------------\n");

    char post_information[SEND_MESSAGE_BUFSIZE];
    char *curr_msg = NULL;
    char METHOD[4] = "";
    char VERSION[10] = "";
    char URL[SEND_MESSAGE_BUFSIZE] = "";

    curr_msg = strtok(msg, "\n");
    
    int line_count = 1;
    while(curr_msg){
        if(line_count>=15) strcpy(post_information, curr_msg);
        curr_msg = strtok(NULL, "\n");
        line_count++;
    }

    firstLine[0] = strtok(msg, " \t\n");
    firstLine[1] = strtok(NULL, " \t");
    firstLine[2] = strtok(NULL, " \t\n");

    strcpy(METHOD, firstLine[0]);
    strcpy(URL, firstLine[1]);
    strcpy(VERSION, firstLine[2]);

    if(!strncmp(METHOD, "GET", 3)) return 0; // error except
}

void GET_handler(char *V, char *message, char *U, int client){
    int fd, str_len;
    char SEND_DATA[SEND_MESSAGE_BUFSIZE];
    char FINAL_PATH[BUFSIZE];
    char VERSION[10]="";
    char URL[SEND_MESSAGE_BUFSIZE]="";

    strcpy(VERSION, V);
    strcpy(URL, U);

    if(strncmp(VERSION, "HTTP/1.0", 8)!=0 && strncmp(VERSION, "HTTP/1.1", 8)!=0){
        write(client, "HTTP/1.1 400 Bad Request\n", 25);
    }
    if(strlen(URL) == 1 && !strncmp(URL, "/", 1)) strcpy(URL, "/index.html");
}
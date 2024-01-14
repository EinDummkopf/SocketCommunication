#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int sockfd;
// socket(int family, int type, int protocol)
if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // use TCP on IPv4
    // PF_INET: Internet protocol (tcp/ip)
    // SOCK_STREAM: TCP
    perror("socket error");
    exit(1);
}
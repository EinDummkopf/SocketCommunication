#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MYPORT 50000

int sockfd;
struct sockaddr_in my_addr;  // sockaddr_in has been defined netinet/in.h
// socket(int family, int type, int protocol)
if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // use TCP on IPv4
    // PF_INET: Internet protocol (tcp/ip)
    // SOCK_STREAM: TCP
    perror("socket error");
    exit(1);
}

memset(&my_addr, 0, sizeof(my_addr)); // reset the socket struct using memset in string.h

my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(MYPORT); // htons converts the number used by the host into shor format
my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY return current my address
/* hton s, l => short, long type */

if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
    perror("bind error");
    exit(1);
}
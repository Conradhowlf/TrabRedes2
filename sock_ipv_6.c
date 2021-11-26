#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include "./lib/common.h"

int socket_ipv_6(int sd, int optval) {
    struct  sockaddr_in6 srv_addr6;

    if ((sd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        perror("socket() failed");
        return -1;
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0) {
        perror("setsockopt(SO_REUSEADOR) failed");
        return -1;
    }

    memset(&srv_addr6, 0, sizeof(srv_addr6));
    srv_addr6.sin6_family = AF_INET6;
    srv_addr6.sin6_port = htons(atoi(FORECAST_PORT));
    srv_addr6.sin6_addr = in6addr_any; 

    if (bind(sd, (struct sockaddr *)&srv_addr6, sizeof(srv_addr6)) < 0) {
        perror("bind() failed");
        return -1;
    }

    return sd;
}
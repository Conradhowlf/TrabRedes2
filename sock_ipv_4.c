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

int socket_ipv_4(int sd, int optval) {
      struct sockaddr_in srv_addr;

      if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
         perror("socket() failed");
         return -1;
      }

      if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0) {
         perror("setsockopt(SO_REUSEADOR) failed");
         return -1;
      }

      memset(&srv_addr, 0, sizeof(srv_addr));
      srv_addr.sin_family = AF_INET;
      srv_addr.sin_port = htons(atoi(FORECAST_PORT));
      srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

      if (bind(sd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
         perror("bind() failed");
         return -1;
      }

      return sd;
}
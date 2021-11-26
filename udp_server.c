/*
  Compilar:
     make
  Sintaxe:
      ./udpserver ipv4 or 
      ./udpserver ipv6   
   
  Ref.:
     https://www.pucpcaldas.br/aularede/mod/page/view.php?id=88757
     https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.1.0/com.ibm.zos.v2r1.hala001/xsocudp.htm
     https://www.softlab.ntua.gr/facilities/documentation/unix/unix-socket-faq/unix-socket-faq-4.html
*/
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
#include "./lib/weather.h"
#include "./lib/sock_ipv_4.h"
#include "./lib/sock_ipv_6.h"

int main(int argc, char *argv[]) {
    struct  sockaddr_in cli_addr;
    struct  sockaddr_in6 cli_addr6;
    char    in[STATION_SIZE], out[TEMPERATURE_SIZE];
    socklen_t client_length = sizeof(struct sockaddr);
    float   min = 0.0, max = 100.0;
    int     sd=-1, pid;
    int     optval = 1, isipv4=0;
    char str[INET6_ADDRSTRLEN];

    if (argc == 2) {
       if (strcmp(argv[1], "ipv4") == 0) {
         isipv4 = 1;
       }
       else {
         if (strcmp(argv[1], "ipv6") == 0) {
            isipv4 = 0;
          }
          else {
             usage();
          }
       }
    }
    else {
       usage();
    }

    do {

      if (isipv4 == 1) {
         sd = socket_ipv_4(sd, optval);
         if (sd == -1) {
            break;
         }         
         

         recvfrom(sd, in, STATION_SIZE, 0,
            (struct sockaddr *)&cli_addr, &client_length);
      }
      else {
         sd = socket_ipv_6(sd, optval);
         if (sd == -1) {
            break;
         }

         recvfrom(sd, in, STATION_SIZE, 0,
	       (struct sockaddr *)&cli_addr6, &client_length);
      }
      
      pid = fork();
      if (pid == 0) {
         
         memset(&out, 0, TEMPERATURE_SIZE);
         sprintf(out, "%5.1f", forecast_for(in, min, max));
         
         if (isipv4 == 1) {
            printf("\nReceived message %s from domain %s internet address %s:%d\n", out,
                  (cli_addr.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
                  inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            sendto(sd, out, TEMPERATURE_SIZE, 0, (struct sockaddr *)&cli_addr, client_length);
         }
         else {
            printf("\nReceived message %s from domain %s internet address %s:%d\n", out,
                  (cli_addr6.sin6_family == AF_INET6?"AF_INET6":"UNKNOWN"),
                  inet_ntop(AF_INET6, &cli_addr6.sin6_addr, str, sizeof(str)), ntohs(cli_addr.sin_port));

            sendto(sd, out, TEMPERATURE_SIZE, 0, (struct sockaddr *)&cli_addr6, client_length);
         }
         
         printf("sendto() success\n");
   
         exit(0);
      }
    } while (1);
    
    if (sd != -1) {
      close(sd);
    }
}

void usage(void) {
     (void)fprintf(stderr, "usage: %s [-s] Enter <ipv4> or <ipv6>\n", __progname);
     exit(1);
}
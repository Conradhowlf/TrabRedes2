all: udpclient udpserver

udpclient: ../udp_client.c ../lib/common.h ../lib/weather.h
	clang -Wall ../udp_client.c -o udpclient
 
udpserver: ../udp_server.c ../lib/common.h ../lib/weather.h ../lib/sock_ipv_4.h ../lib/sock_ipv_6.h
	clang -Wall ../udp_server.c ../weather.c ../sock_ipv_4.c ../sock_ipv_6.c -o udpserver

clean: 
	rm udpclient udpserver
 

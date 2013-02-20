#ifndef _LIBALSHAINBUS_TCP_H_
#define _LIBALSHAINBUS_TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include <stdint.h>


typedef struct {
	
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    unsigned char buffer[256];
    
	} abus_tcp_t;


void abus_tcp_connect( abus_tcp_t *conn );
int abus_tcp_recv( abus_tcp_t *conn );
int abus_tcp_send( abus_tcp_t *conn, unsigned char *buffer, uint8_t size );
void abus_tcp_disconnect( abus_tcp_t *conn );



#endif


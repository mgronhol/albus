#include "libAlbusTCP.h"

void abus_tcp_connect( abus_tcp_t *conn ){
	
	conn->portno = 1222;
	conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	conn->server = gethostbyname( "localhost" );
	
	bzero((char *) &conn->serv_addr, sizeof(conn->serv_addr));
	
	conn->serv_addr.sin_family = AF_INET;
    bcopy((char *)conn->server->h_addr, 
         (char *)&conn->serv_addr.sin_addr.s_addr,
         conn->server->h_length);
	
	conn->serv_addr.sin_port = htons(conn->portno);
	
	connect(conn->sockfd,(struct sockaddr *) &conn->serv_addr,sizeof(conn->serv_addr));
	
	fcntl(conn->sockfd, F_SETFL, O_NONBLOCK);
	
	}

int abus_tcp_recv( abus_tcp_t *conn ){
	int n;
	memset( conn->buffer, 0, 256 );
	
	n = read( conn->sockfd, conn->buffer, 256 );
	if( n > 0 ){ return n; }
	return 0;
	}

int abus_tcp_send( abus_tcp_t *conn, unsigned char *buffer, uint8_t size ){
	int n;
	n = write( conn->sockfd, buffer, size );
	return n;
	}


void abus_tcp_disconnect( abus_tcp_t *conn ){
	close( conn->sockfd );
	}


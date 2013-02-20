#include <stdio.h>

#include "libAlshainbus.h"

#include "libAbusTCP.h"


int main(){
	
	abus_t conn;
	abus_tcp_t bus;
	unsigned char *buf;
	uint8_t size;
	int done = 0;
	
	abus_init( &conn, 1 );
	
	abus_tcp_connect( &bus );
	
	abus_set_recipient( &conn, 100 );
	abus_set_flags( &conn, ABUS_OPT_REQUEST );
	abus_set_params( &conn, NULL, 0 );
	abus_set_function( &conn, ABUS_FUNC_IDENT );
	
	size = abus_prepare( &conn );
	
	buf = abus_get_buffer( &conn );
	
	abus_tcp_send( &bus, buf, size );
	
	while( !done ){
		int i, rc;
		int n = abus_tcp_recv( &bus );
		if( n > 0 ){
			for( i = 0 ; i < n ; ++i ){
				rc = abus_read( &conn, bus.buffer[i] );
				if( rc == ABUS_MSG_DONE ){
					done = 1;
					break;
					}
				}
			}
		}
	
	{
		int i;
		printf( "buffer: ");
		for( i = 0 ; i < ABUS_BUFFER_SIZE ; ++i ){
			printf( "%02x ", conn.buffer[i] );
			}
		printf( "\n\n" );
		}
	
	printf( "idn: %s\n", abus_get_params( &conn ) );
	printf( "size: %i\n", abus_get_size( &conn ) );
	
	abus_tcp_disconnect( &bus );
	return 0;
	}

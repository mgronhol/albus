#include <stdio.h>

#include "libAlshainbus.h"

#include "libAlbusTCP.h"


int main(){
	
	albus_t conn;
	albus_tcp_t bus;
	unsigned char *buf;
	uint8_t size;
	int done = 0;
	
	albus_init( &conn, 1 );
	
	albus_tcp_connect( &bus );
	
	albus_set_recipient( &conn, 100 );
	albus_set_flags( &conn, ALBUS_OPT_REQUEST );
	albus_set_params( &conn, NULL, 0 );
	albus_set_function( &conn, ALBUS_FUNC_IDENT );
	
	size = albus_prepare( &conn );
	
	buf = albus_get_buffer( &conn );

	{
		int i;
		for( i = 0 ; i < size ; ++i ){
			printf( "%02x ", buf[i] );
			}
		printf( "\n" );
		}
	
	albus_tcp_send( &bus, buf, size );
	
	while( !done ){
		int i, rc;
		int n = albus_tcp_recv( &bus );
		if( n > 0 ){
			for( i = 0 ; i < n ; ++i ){
				rc = albus_read( &conn, bus.buffer[i] );
				if( rc == ALBUS_MSG_DONE ){
					done = 1;
					break;
					}
				}
			}
		}
	
	{
		int i;
		printf( "buffer: ");
		for( i = 0 ; i < ALBUS_BUFFER_SIZE ; ++i ){
			printf( "%02x ", conn.buffer[i] );
			}
		printf( "\n\n" );
		}
	
	printf( "idn: %s\n", albus_get_params( &conn ) );
	printf( "size: %i\n", albus_get_size( &conn ) );
	
	albus_tcp_disconnect( &bus );
	return 0;
	}

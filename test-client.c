#include <stdio.h>

#include "libAlshainbus.h"

#include "libAlbusTCP.h"

const char IDENT[] = "TC-001";

int main(){
	
	albus_t conn;
	albus_tcp_t bus;
	unsigned char *buf;
	uint8_t size;
	int done = 0;
	
	albus_init( &conn, 100 );
	
	albus_tcp_connect( &bus );
	
	while( 1 ){
		albus_reset( &conn );
		done = 0;
		while( !done ){
			int i, rc;
			int n = albus_tcp_recv( &bus );
			if( n > 0 ){
				for( i = 0 ; i < n ; ++i ){
					rc = albus_read( &conn, bus.buffer[i] );
					if( rc == ALBUS_MSG_DONE || rc == ALBUS_MSG_INVALID ){
						done = 1;
						break;
						}
					}
				}
			
			if( rc == ALBUS_MSG_INVALID ){
				done = 0;
				albus_reset( &conn );
				}
			
			}
		
		if( albus_get_recipient( &conn ) != ALBUS_NODE_BROADCAST && albus_get_recipient( &conn ) != conn.node_id ){
			continue;
			}
		
		switch( albus_get_function( &conn ) ){
			case ALBUS_FUNC_IDENT:
				{
				albus_set_recipient( &conn, ALBUS_NODE_MASTER );
				albus_set_flags( &conn, ALBUS_OPT_RESPONSE );
				albus_set_params( &conn, (unsigned char*)IDENT, strlen( IDENT ) );
				
				size = albus_prepare( &conn );
				buf = albus_get_buffer( &conn );
				
				{
					int z;
					printf( "response: " );
					for( z = 0 ; z < size ; ++z ){
						printf( "%02x ", buf[z] );
						}
					printf( "\n\n" );
					}
				
				albus_tcp_send( &bus, buf, size );
				
				}
			break;
			
			default: break;
			}
		}
	
	
	albus_tcp_disconnect( &bus );

	return 0;
	}

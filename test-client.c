#include <stdio.h>

#include "libAlshainbus.h"

#include "libAlbusTCP.h"

const char IDENT[] = "TC-001";

int main(){
	
	abus_t conn;
	abus_tcp_t bus;
	unsigned char *buf;
	uint8_t size;
	int done = 0;
	
	abus_init( &conn, 100 );
	
	abus_tcp_connect( &bus );
	
	while( 1 ){
		abus_reset( &conn );
		done = 0;
		while( !done ){
			int i, rc;
			int n = abus_tcp_recv( &bus );
			if( n > 0 ){
				for( i = 0 ; i < n ; ++i ){
					rc = abus_read( &conn, bus.buffer[i] );
					if( rc == ABUS_MSG_DONE || rc == ABUS_MSG_INVALID ){
						done = 1;
						break;
						}
					}
				}
			
			if( rc == ABUS_MSG_INVALID ){
				done = 0;
				abus_reset( &conn );
				}
			
			}
		
		if( abus_get_recipient( &conn ) != ABUS_NODE_BROADCAST && abus_get_recipient( &conn ) != conn.node_id ){
			continue;
			}
		
		switch( abus_get_function( &conn ) ){
			case ABUS_FUNC_IDENT:
				{
				abus_set_recipient( &conn, ABUS_NODE_MASTER );
				abus_set_flags( &conn, ABUS_OPT_RESPONSE );
				abus_set_params( &conn, (unsigned char*)IDENT, strlen( IDENT ) );
				
				size = abus_prepare( &conn );
				buf = abus_get_buffer( &conn );
				
				{
					int z;
					printf( "response: " );
					for( z = 0 ; z < size ; ++z ){
						printf( "%02x ", buf[z] );
						}
					printf( "\n\n" );
					}
				
				abus_tcp_send( &bus, buf, size );
				
				}
			break;
			
			default: break;
			}
		}
	
	
	abus_tcp_disconnect( &bus );

	return 0;
	}

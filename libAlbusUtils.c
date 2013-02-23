#include "libAlbusUtils.h"



void abus_loop_init( abus_loop_t *loop, uint8_t id, abus_callback_t cb, void *args ){
	
	abus_init( &loop->conn, id );
	abus_tcp_connect( &loop->bus );
	
	loop->state = 0;
	
	loop->callback = cb;
	
	loop->args = args;
	
	}


void abus_loop_infinite( abus_loop_t *loop ){
		int done;
		int retval;
		
		while( 1 ){
			abus_reset( &loop->conn );
			done = 0;
			while( !done ){
				int i, rc = ABUS_MSG_CONT;
				int n = abus_tcp_recv( &loop->bus );
				if( n > 0 ){
					for( i = 0 ; i < n ; ++i ){
						rc = abus_read( &loop->conn, loop->bus.buffer[i] );
						if( rc == ABUS_MSG_DONE || rc == ABUS_MSG_INVALID ){
							done = 1;
							break;
							}
						}
					}
				
				if( rc == ABUS_MSG_INVALID ){
					done = 0;
					abus_reset( &loop->conn );
					}
				
				}
			
			if( abus_get_recipient( &loop->conn ) != ABUS_NODE_BROADCAST && abus_get_recipient( &loop->conn ) != loop->conn.node_id ){
				continue;
				}
			
			
			retval = loop->callback( &loop->conn, loop->args );
			
			if( retval != ABUS_RETVAL_EMPTY ){
				unsigned char *buf;
				uint8_t size;
	
				size = abus_prepare( &loop->conn );
				buf = abus_get_buffer( &loop->conn );
				
				abus_tcp_send( &loop->bus, buf, size );

				}
			
		}
	
	}


void abus_loop_single( abus_loop_t *loop ){
	
	
	switch( loop->state ){
		case 0:
			{
			abus_reset( &loop->conn );	
			loop->state = 1;
			}
		break;
		
		case 1:
			{
			int i, rc = ABUS_MSG_CONT;
			int n = abus_tcp_recv( &loop->bus );
			if( n > 0 ){
				for( i = 0 ; i < n ; ++i ){
					rc = abus_read( &loop->conn, loop->bus.buffer[i] );
					if( rc == ABUS_MSG_DONE || rc == ABUS_MSG_INVALID ){
						loop->state = 2;
						break;
						}
					}
				}
			
			
			
			if( rc == ABUS_MSG_INVALID && n > 0 ){
				loop->state = 0;
				}	
			}
		break;
		
		case 2:
			{
			if( abus_get_recipient( &loop->conn ) != ABUS_NODE_BROADCAST && abus_get_recipient( &loop->conn ) != loop->conn.node_id ){
				loop->state = 0;
				}
			else {
				loop->state = 3;
				}
			}
		break;
		
		case 3:
			{
			int retval = loop->callback( &loop->conn, loop->args );
			
			if( retval != ABUS_RETVAL_EMPTY ){
				unsigned char *buf;
				uint8_t size;
				size = abus_prepare( &loop->conn );
				buf = abus_get_buffer( &loop->conn );
				
				abus_tcp_send( &loop->bus, buf, size );

				}
			loop->state = 4;
			}
		break;
		
		case 4:
			loop->state = 0;
		break;
		
		default:
			loop->state = 0;
		break;
		
		}
	
	
	}

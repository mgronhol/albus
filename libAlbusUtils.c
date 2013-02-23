#include "libAlbusUtils.h"



void albus_loop_init( albus_loop_t *loop, uint8_t id, albus_callback_t cb, void *args ){
	
	albus_init( &loop->conn, id );
	albus_tcp_connect( &loop->bus );
	
	loop->state = 0;
	
	loop->callback = cb;
	
	loop->args = args;
	
	}


void albus_loop_infinite( albus_loop_t *loop ){
		int done;
		int retval;
		
		while( 1 ){
			albus_reset( &loop->conn );
			done = 0;
			while( !done ){
				int i, rc = ALBUS_MSG_CONT;
				int n = albus_tcp_recv( &loop->bus );
				if( n > 0 ){
					for( i = 0 ; i < n ; ++i ){
						rc = albus_read( &loop->conn, loop->bus.buffer[i] );
						if( rc == ALBUS_MSG_DONE || rc == ALBUS_MSG_INVALID ){
							done = 1;
							break;
							}
						}
					}
				
				if( rc == ALBUS_MSG_INVALID ){
					done = 0;
					albus_reset( &loop->conn );
					}
				
				}
			
			if( albus_get_recipient( &loop->conn ) != ALBUS_NODE_BROADCAST && albus_get_recipient( &loop->conn ) != loop->conn.node_id ){
				continue;
				}
			
			
			retval = loop->callback( &loop->conn, loop->args );
			
			if( retval != ALBUS_RETVAL_EMPTY ){
				unsigned char *buf;
				uint8_t size;
	
				size = albus_prepare( &loop->conn );
				buf = albus_get_buffer( &loop->conn );
				
				albus_tcp_send( &loop->bus, buf, size );

				}
			
		}
	
	}


void albus_loop_single( albus_loop_t *loop ){
	
	
	switch( loop->state ){
		case 0:
			{
			albus_reset( &loop->conn );	
			loop->state = 1;
			}
		break;
		
		case 1:
			{
			int i, rc = ALBUS_MSG_CONT;
			int n = albus_tcp_recv( &loop->bus );
			if( n > 0 ){
				for( i = 0 ; i < n ; ++i ){
					rc = albus_read( &loop->conn, loop->bus.buffer[i] );
					if( rc == ALBUS_MSG_DONE || rc == ALBUS_MSG_INVALID ){
						loop->state = 2;
						break;
						}
					}
				}
			
			
			
			if( rc == ALBUS_MSG_INVALID && n > 0 ){
				loop->state = 0;
				}	
			}
		break;
		
		case 2:
			{
			if( albus_get_recipient( &loop->conn ) != ALBUS_NODE_BROADCAST && albus_get_recipient( &loop->conn ) != loop->conn.node_id ){
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
			
			if( retval != ALBUS_RETVAL_EMPTY ){
				unsigned char *buf;
				uint8_t size;
				size = albus_prepare( &loop->conn );
				buf = albus_get_buffer( &loop->conn );
				
				albus_tcp_send( &loop->bus, buf, size );

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

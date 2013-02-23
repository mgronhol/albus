#include "libAlshainbus.h"

#include <stdio.h>

void albus_init( albus_t *conn, uint8_t id ){
	conn->node_id = id;
	conn->size = 0;
	conn->pos = 0;
	}

void albus_reset( albus_t *conn ){
	conn->size = 0;
	conn->pos = 0;
	}

int albus_read( albus_t *conn, unsigned char byte ){
	if( conn->pos == 0 ){
		if( byte != 0 && byte != conn->node_id ){
			return ALBUS_MSG_NOT_FOR_US;
			} 
		}
	conn->buffer[ conn->pos ] = byte;
	
	if( conn->pos == 3 ){
		conn->size = byte;
		}
	
	conn->pos += 1;
	
	if( conn->pos >= ALBUS_BUFFER_SIZE ){
		return ALBUS_MSG_INVALID;
		}
	
	if( conn->pos > 4 + conn->size ){
		uint8_t cs = albus_compute_checksum( conn );
		if( cs == conn->buffer[conn->pos-1] ){
			return ALBUS_MSG_DONE;
			}
		return ALBUS_MSG_INVALID;
		}
	return ALBUS_MSG_CONT;
	}

uint8_t albus_get_recipient( albus_t *conn ){
	return (uint8_t)conn->buffer[0];
	}

uint8_t albus_get_flags( albus_t *conn ){
	return (uint8_t)conn->buffer[1];
	}

uint8_t albus_get_function( albus_t *conn ){
	return (uint8_t)conn->buffer[2];
	}

uint8_t albus_get_size( albus_t *conn ){
	return conn->size;
	}

unsigned char* albus_get_params( albus_t *conn ){
	return conn->buffer + 4;
	}

void albus_set_recipient( albus_t *conn, uint8_t id ){
	conn->buffer[0] = (unsigned char)id;
	}

void albus_set_flags( albus_t *conn, uint8_t flags ){
	conn->buffer[1] = (unsigned char)flags;
	}

void albus_set_function( albus_t *conn, uint8_t function ){
	conn->buffer[2] = (unsigned char)function;
	}


void albus_set_params( albus_t *conn, unsigned char *params, uint8_t size ){
	int i;
	for( i = 0 ; i < size ; ++i ){
		conn->buffer[4+i] = params[i];
		}
	conn->size = size;
	conn->buffer[3] = conn->size;
	}

uint8_t albus_compute_checksum( albus_t *conn ){
	uint8_t out = 0;
	int i;
	for( i = 0 ; i < conn->size + 4 ; ++i ){
		out ^= conn->buffer[i];
		}
		
	return out;
	}

unsigned char* albus_get_buffer( albus_t *conn ){
	return conn->buffer;
	}

uint8_t albus_prepare( albus_t *conn ){
	conn->buffer[conn->size + 4] = albus_compute_checksum( conn );
	
	return conn->size + 5;
	}

/*
#include <stdio.h>


int main(){
	//printf( "size: %lu \n", sizeof( albus_t ) );
	albus_t conn, conn2;
	int i;
	int rc;
	uint8_t size;
	unsigned char *buf;
	
	albus_init( &conn, 1 );
	albus_init( &conn2, 100 );
	
	
	albus_set_recipient( &conn, 100 );
	albus_set_flags( &conn, ABUS_OPT_REQUEST );
	albus_set_params( &conn, NULL, 0 );
	albus_set_function( &conn, ABUS_FUNC_IDENT );
	
	size = albus_prepare( &conn );
	
	buf = albus_get_buffer( &conn );
	
	for( i = 0 ; i < size ; ++i ){
		rc = albus_read( &conn2, buf[i] );
		printf( "rc: %i\n", rc );
		}
	
	printf( "func code: %i\n", albus_get_function( &conn2  ) );
	
	return 0;
	}
*/

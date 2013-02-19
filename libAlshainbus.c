#include "libAlshainbus.h"

void abus_init( abus_t *conn, uint8_t id ){
	conn->node_id = id;
	conn->size = 0;
	conn->pos = 0;
	}

void abus_reset( abus_t *conn ){
	conn->size = 0;
	conn->pos = 0;
	}

int abus_read( abus_t *conn, unsigned char byte ){
	if( conn->pos == 0 ){
		if( byte != 0 && byte != conn->node_id ){
			return ABUS_MSG_NOT_FOR_US;
			} 
		}
	conn->buffer[ conn->pos ] = byte;
	
	if( conn->pos == 3 ){
		conn->size = byte;
		}
	
	conn->pos += 1;
	
	if( conn->pos >= ABUS_BUFFER_SIZE ){
		return ABUS_MSG_INVALID;
		}
	
	if( conn->pos > 5 + conn->size ){
		uint8_t cs = abus_compute_checksum( conn );
		if( cs == conn->buffer[conn->pos-1] ){
			return ABUS_MSG_DONE;
			}
		return ABUS_MSG_INVALID;
		}
	return ABUS_MSG_CONT;
	}

uint8_t abus_get_recipient( abus_t *conn ){
	return (uint8_t)conn->buffer[0];
	}

uint8_t abus_get_flags( abus_t *conn ){
	return (uint8_t)conn->buffer[1];
	}

uint8_t abus_get_function( abus_t *conn ){
	return (uint8_t)conn->buffer[2];
	}

uint8_t abus_get_size( abus_t *conn ){
	return conn->size;
	}

unsigned char* abus_get_params( abus_t *conn ){
	return conn->buffer + 4;
	}

void abus_set_recipient( abus_t *conn, uint8_t id ){
	conn->buffer[0] = (unsigned char)id;
	}

void abus_set_flags( abus_t *conn, uint8_t flags ){
	conn->buffer[1] = (unsigned char)flags;
	}

void abus_set_function( abus_t *conn, uint8_t function ){
	conn->buffer[2] = (unsigned char)function;
	}


void abus_set_params( abus_t *conn, unsigned char *params, uint8_t size ){
	int i;
	for( i = 0 ; i < size ; ++i ){
		conn->buffer[4+i] = params[i];
		}
	conn->size = size;
	}

uint8_t abus_compute_checksum( abus_t *conn ){
	uint8_t out = 0;
	int i;
	for( i = 0 ; i < conn->size + 4 ; ++i ){
		out ^= conn->buffer[i];
		}
	
	}

unsigned char* abus_get_buffer( abus_t *conn ){
	return conn->buffer;
	}

uint8_t abus_prepare( abus_t *conn ){
	conn->buffer[conn->size + 5] = abus_compute_checksum( conn );
	return conn->size + 6;
	}

#include <stdio.h>


int main(){
	//printf( "size: %lu \n", sizeof( abus_t ) );
	abus_t conn, conn2;
	int i;
	int rc;
	uint8_t size;
	unsigned char *buf;
	
	abus_init( &conn, 1 );
	abus_init( &conn2, 100 );
	
	
	abus_set_recipient( &conn, 100 );
	abus_set_flags( &conn, ABUS_OPT_REQUEST );
	abus_set_params( &conn, NULL, 0 );
	abus_set_function( &conn, ABUS_FUNC_IDENT );
	
	size = abus_prepare( &conn );
	
	buf = abus_get_buffer( &conn );
	
	for( i = 0 ; i < size ; ++i ){
		rc = abus_read( &conn2, buf[i] );
		printf( "rc: %i\n", rc );
		}
	
	printf( "func code: %i\n", abus_get_function( &conn2  ) );
	
	return 0;
	}

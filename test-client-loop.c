#include  <stdio.h>

#include "libAbusUtils.h"

const char IDENT[] = "TC-001";


int handler_callback( abus_t *conn , void *args ){
	abus_set_recipient( conn, ABUS_NODE_MASTER );
	abus_set_flags( conn, ABUS_OPT_RESPONSE );
	abus_set_params( conn, (unsigned char*)IDENT, strlen( IDENT ) );
	return ABUS_RETVAL_OK;			
	}

int main(){
	
	abus_loop_t loop;
	
	abus_loop_init( &loop, 100, &handler_callback, NULL );
	
	/*
	while( 1 ){
		abus_loop_single( &loop );
		}
	*/
	abus_loop_infinite( &loop );
	
	return 0;
	}

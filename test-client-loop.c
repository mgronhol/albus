#include  <stdio.h>

#include "libAlbusUtils.h"

const char IDENT[] = "TC-001";


int handler_callback( albus_t *conn , void *args ){
	albus_set_recipient( conn, ALBUS_NODE_MASTER );
	albus_set_flags( conn, ALBUS_OPT_RESPONSE );
	albus_set_params( conn, (unsigned char*)IDENT, strlen( IDENT ) );
	return ALBUS_RETVAL_OK;			
	}

int main(){
	
	albus_loop_t loop;
	
	albus_loop_init( &loop, 100, &handler_callback, NULL );
	
	/*
	while( 1 ){
		albus_loop_single( &loop );
		}
	*/
	albus_loop_infinite( &loop );
	
	return 0;
	}

#ifndef _LIBALBUSUTILSH_H_
#define _LIBALBUSUTILSH_H_

#include "libAlshainbus.h"
#include "libAlbusTCP.h"

#define ALBUS_RETVAL_EMPTY	-1
#define ALBUS_RETVAL_OK		0


typedef int (*albus_callback_t)( albus_t *, void * );


typedef struct {
	albus_t conn;
	albus_tcp_t bus;
	albus_callback_t callback;
	
	void *args;
	
	uint8_t state;
	} albus_loop_t;


void albus_loop_init( albus_loop_t *loop, uint8_t id, albus_callback_t cb, void *args );

void albus_loop_infinite( albus_loop_t *loop );

void albus_loop_single( albus_loop_t *loop );

#endif

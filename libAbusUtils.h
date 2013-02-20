#ifndef _LIBABUSUTILSH_H_
#define _LIBABUSUTILSH_H_

#include "libAlshainbus.h"
#include "libAbusTCP.h"

#define ABUS_RETVAL_EMPTY	-1
#define ABUS_RETVAL_OK		0


typedef int (*abus_callback_t)( abus_t *, void * );


typedef struct {
	abus_t conn;
	abus_tcp_t bus;
	abus_callback_t callback;
	
	void *args;
	
	uint8_t state;
	} abus_loop_t;


void abus_loop_init( abus_loop_t *loop, uint8_t id, abus_callback_t cb, void *args );

void abus_loop_infinite( abus_loop_t *loop );

void abus_loop_single( abus_loop_t *loop );


#endif

#ifndef _LIBALSHAINBUS_H_
#define _LIBALSHAINBUS_H_

#include <stdint.h>

#define ALBUS_NODE_BROADCAST	0
#define ALBUS_NODE_MASTER		1

#define ALBUS_FLAG_REQREP		0

#define ALBUS_FUNC_READ_PORT		0
#define ALBUS_FUNC_WRITE_PORT	1
#define ALBUS_FUNC_READ_ANALOG	2
#define ALBUS_FUNC_WRITE_ANALOG	3
#define ALBUS_FUNC_READ_REG		4
#define ALBUS_FUNC_WRITE_REG	5
#define ALBUS_FUNC_UNFREEZE		0xfd
#define ALBUS_FUNC_FREEZE		0xfe
#define ALBUS_FUNC_IDENT		0xff

#define ALBUS_STATE_UNFROZEN	0
#define ALBUS_STATE_FROZEN		1

#define ALBUS_MSG_CONT			0
#define ALBUS_MSG_DONE			1
#define ALBUS_MSG_NOT_FOR_US	2

#define ALBUS_MSG_OK			4
#define ALBUS_MSG_INVALID		5

#define ALBUS_OPT_REQUEST		0
#define ALBUS_OPT_RESPONSE		1


#define ALBUS_BUFFER_SIZE		32

typedef struct {
	uint8_t node_id;
	uint8_t size, pos;
	unsigned char buffer[ ALBUS_BUFFER_SIZE ];
	} albus_t;

void albus_init( albus_t *conn, uint8_t id );
void albus_reset( albus_t *conn );

int albus_read( albus_t *conn, unsigned char byte );

uint8_t albus_get_recipient( albus_t *conn );
uint8_t albus_get_flags( albus_t *conn );
uint8_t albus_get_function( albus_t *conn );
uint8_t albus_get_size( albus_t *conn );
unsigned char* albus_get_params( albus_t *conn );

void albus_set_recipient( albus_t *conn, uint8_t id );
void albus_set_function( albus_t *conn, uint8_t function );
void albus_set_flags( albus_t *conn, uint8_t flags );
void albus_set_params( albus_t *conn, unsigned char *params, uint8_t size );
uint8_t albus_prepare( albus_t *conn );

uint8_t albus_compute_checksum( albus_t *conn );

unsigned char* albus_get_buffer( albus_t *conn );

#endif

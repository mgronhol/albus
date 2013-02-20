#ifndef _LIBALSHAINBUS_H_
#define _LIBALSHAINBUS_H_

#include <stdint.h>

#define ABUS_NODE_BROADCAST		0
#define ABUS_NODE_MASTER		1

#define ABUS_FLAG_REQREP		0

#define ABUS_FUNC_READ_PORT		0
#define ABUS_FUNC_WRITE_PORT	1
#define ABUS_FUNC_READ_ANALOG	2
#define ABUS_FUNC_WRITE_ANALOG	3
#define ABUS_FUNC_READ_REG		4
#define ABUS_FUNC_WRITE_REG		5
#define ABUS_FUNC_UNFREEZE		0xfd
#define ABUS_FUNC_FREEZE		0xfe
#define ABUS_FUNC_IDENT			0xff

#define ABUS_STATE_UNFROZEN		0
#define ABUS_STATE_FROZEN		1

#define ABUS_MSG_CONT			0
#define ABUS_MSG_DONE			1
#define ABUS_MSG_NOT_FOR_US		2

#define ABUS_MSG_OK				4
#define ABUS_MSG_INVALID		5

#define ABUS_OPT_REQUEST		0
#define ABUS_OPT_RESPONSE		1


#define ABUS_BUFFER_SIZE		32

typedef struct {
	uint8_t node_id;
	uint8_t size, pos;
	unsigned char buffer[ ABUS_BUFFER_SIZE ];
	} abus_t;

void abus_init( abus_t *conn, uint8_t id );
void abus_reset( abus_t *conn );

int abus_read( abus_t *conn, unsigned char byte );

uint8_t abus_get_recipient( abus_t *conn );
uint8_t abus_get_flags( abus_t *conn );
uint8_t abus_get_function( abus_t *conn );
uint8_t abus_get_size( abus_t *conn );
unsigned char* abus_get_params( abus_t *conn );

void abus_set_recipient( abus_t *conn, uint8_t id );
void abus_set_function( abus_t *conn, uint8_t function );
void abus_set_flags( abus_t *conn, uint8_t flags );
void abus_set_params( abus_t *conn, unsigned char *params, uint8_t size );
uint8_t abus_prepare( abus_t *conn );

uint8_t abus_compute_checksum( abus_t *conn );

unsigned char* abus_get_buffer( abus_t *conn );

#endif

/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#ifndef __OSCAR_FLAP_H__
#define __OSCAR_FLAP_H__

#include <stdint.h>	/* ?int*_t */ 

typedef struct _oscar_flap_header oscar_flap_header;

struct _oscar_flap_header
{
	uint8_t id;		/* Always 2A */
	uint8_t channel;	/* FLAP channel */
	uint16_t seq;		/* Sequence number */
	uint16_t size;		/* Size of data */
	void *data;		/* Pointer to data */
};

/* Standard FLAP channels */
#define OSCAR_FLAP_CONN_NEGOTIATE	0x1
#define OSCAR_FLAP_SNAC_DATA		0x2
#define OSCAR_FLAP_ERROR		0x3
#define OSCAR_FLAP_CLOSE_NEGOTIATE	0x4
#define OSCAR_FLAP_KEEPALIVE		0x5

/* Size of FLAP header minus data */
#define OSCAR_FLAP_SIZE 48

extern char * oscar_flap_create(oscar *o, char *data, uint16_t size, uint8_t channel);
extern void oscar_flap_free(oscar *o, oscar_flap_header *header);
extern char * oscar_flap_from_header(oscar *o, oscar_flap_header *header);
extern void oscar_flap_header_read(oscar *o, char *data, oscar_flap_header *header);
extern uint64_t oscar_flap_data_read(oscar *o, char *data, uint64_t size, oscar_flap_header *header);

#endif

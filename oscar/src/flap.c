/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#include <string.h>	/* memcpy/memmove */
#include <stdint.h>	/* ?int*_t */
#include <arpa/inet.h>	/* htons, ntohs */
#include <netinet/in.h>	/* htons, ntohs (some platforms) */
#include <assert.h>

#include "generic/include/libchloe.h"
#include "oscar/include/oscar.h"
#include "oscar/include/flap.h"

/*
 * oscar_flap_create - encapsulate a packet in FLAP
 * in: oscar handle, data to encapsulate, channel, size
 * out: FLAP packet - it will always be OSCAR_FLAP_SIZE + size
 */
char * oscar_flap_create(oscar *o, char *data, uint16_t size, uint8_t channel)
{
	char *packet;

	assert(data != NULL);
	assert(size);

	LCH_CALLOC(packet, OSCAR_FLAP_SIZE + size, 1);

	packet[0] = 0x2A; /* Start of header */
	packet[1] = channel;
	*((uint16_t *)&packet[2]) = htons((o->seq)++); /* Rollover is ok :) */
	*((uint16_t *)&packet[4]) = htons(size); /* Skip a byte */

	/* Copy the data :3 */
	memcpy(&packet[6], data, size);

	return packet;
}

/*
 * oscar_flap_free - free a FLAP packet
 * in: oscar handle, FLAP packet
 * out: None
 */
void oscar_flap_free(oscar *o, oscar_flap_header *header)
{
	LCH_FREE(header->data);
}

/*
 * oscar_flap_from_header - create a FLAP packet from a header
 * in: oscar handle, FLAP header
 * out: FLAP packet
 */
char * oscar_flap_from_header(oscar *o, oscar_flap_header *header)
{
	/* Ignore other crud like the sequence number for consistency */
	return oscar_flap_create(o, header->data, header->size, header->channel);
}

/*
 * oscar_flap_header_read - read a FLAP header
 * in: data containing FLAP header - must be >= OSCAR_FLAP_SIZE bytes, empty
 *     FLAP header
 * out: None 
 */
void oscar_flap_header_read(oscar *o, char *data, oscar_flap_header *header)
{
	assert(data != NULL);
	assert(header != NULL);

	(void)o;

	header->id = data[0];
	header->channel = data[1];
	header->seq = ntohs(*((uint16_t *)&data[2]));
	header->size = ntohs(*((uint16_t *)&data[4])); /* Skip a byte */

	assert(header->id == 0x2A);
}

/*
 * oscar_flap_data_read - read a FLAP packet with data
 * in: data containing FLAP header - must be > OSCAR_FLAP_SIZE bytes, length
 *     of data, empty FLAP header
 * out: length of packet on success, -1 on insufficent data
 */
uint64_t oscar_flap_data_read(oscar *o, char *data, uint64_t size, oscar_flap_header *header)
{
	assert(size > OSCAR_FLAP_SIZE);
	assert(data != NULL);
	assert(header != NULL);

	/* Parse header */
	oscar_flap_header_read(o, data, header);

	if ((size - OSCAR_FLAP_SIZE) < header->size)
		LCH_ERROR(-1, "Insufficient data for FLAP read\n");

	/* Read in data */
	data += OSCAR_FLAP_SIZE;
	LCH_CALLOC(header->data, header->size, 1);
	memcpy(header->data, data, header->size);

	return header->size + OSCAR_FLAP_SIZE;
}

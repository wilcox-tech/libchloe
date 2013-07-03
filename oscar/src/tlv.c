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
#include "oscar/include/tlv.h"

/* Dead simple format... nothing to it */

/*
 * oscar_tlv_read - read a TLV from the given data - must contain all the data
 * in: data, size, empty TLV struct
 * out: -1 on insufficent data, otherwise length
 */
int64_t oscar_tlv_read(const char *data, uint64_t size, oscar_tlv *tlv)
{
	assert(data != NULL);
	assert(tlv != NULL);

	if (size <= OSCAR_TLV_HEADER)
		LCH_ERROR(-1, "Insufficient data - no TLV header\n");

	tlv->type = ntohs(*((uint16_t *)&data[0]));
	tlv->length = ntohs(*((uint16_t *)&data[2]));

	if ((size -= OSCAR_TLV_HEADER) < tlv->length)
		LCH_ERROR(-1, "Insufficient data - not enough TLV data\n");

	/* XXX - bleah, would like to do in-place parsing */
	LCH_CALLOC(tlv->data, tlv->length, 1);
	memcpy(tlv->data, &data[4], tlv->length);

	return 0;
}

/*
 * oscar_tlv_free - free a TLV's internal data
 * in: TLV struct
 * out: None
 */
void oscar_tlv_free(oscar_tlv *tlv)
{
	LCH_FREE(tlv->data);
}

/*
 * oscar_tlv_read_chain - read a TLV chain
 * in: data, max size, tlv chain ptr (will be allocated along with all nodes) 
 * out: -1 on insufficent data, otherwise length of bytes of data consumed
 */
int64_t oscar_tlv_read_chain(const char *data, uint64_t size, oscar_tlv_chain **tlvchain)
{
	oscar_tlv_chain *node = NULL;
	uint64_t cursize = 0;

	*tlvchain = NULL;

	while (cursize < size)
	{
		oscar_tlv tlv;
		oscar_tlv_chain *nextnode;
		int64_t s = oscar_tlv_read(data, size - cursize, &tlv);

		if (s <= 0)
		{
			if (*tlvchain) oscar_tlv_delete_chain(*tlvchain);
			return -1;
		}

		LCH_CALLOC(nextnode, sizeof(oscar_tlv_chain), 1);

		/* XXX - Bleah, this sucks */
		memcpy(&(nextnode->tlv), &tlv, sizeof(oscar_tlv));

		if (node)
			node->next = nextnode;
		else
			/* Initalise start of chain */
			*tlvchain = nextnode;

		node = nextnode;

		cursize += s;
		data += s;
	}

	return (int64_t)cursize;
}

/*
 * oscar_tlv_delete_chain - delete a TLV chain
 * in: TLV chain start
 * out: none
 */
void oscar_tlv_delete_chain(oscar_tlv_chain *tlvchain)
{
	oscar_tlv_chain *tlv, *tlv2;

	/* safely iterate through the list and delete nodes. self-explanatory right? */
	for (tlv = tlvchain, tlv2 = (tlv != NULL ? tlv->next : NULL);
		tlv != NULL;
		tlv = tlv2, tlv2 = (tlv != NULL ? tlv->next : NULL))
	{
		LCH_FREE(tlv);
	}
}

/*
 * oscar_tlv_create - create a TLV tuple from the given data
 * in: type, length, data
 * out: formed TLV packet
 */
char * oscar_tlv_create(const char *data, uint16_t type, uint16_t size)
{
	char *tlv;

	assert(data != NULL);
	assert(type);
	assert(size);

	LCH_CALLOC(tlv, OSCAR_TLV_HEADER + size, 1);

	*((uint16_t *)&tlv[0]) = htons(type);
	*((uint16_t *)&tlv[2]) = htons(size);
	memcpy(&tlv[4], data, size);

	return tlv;
}


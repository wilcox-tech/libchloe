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
#include "oscar/include/snac.h"

/* snac related stuff */

/*
 * oscar_snac_read - read a SNAC struct
 * in: data, size, empty SNAC struct
 * out: -1 on insufficent data, otherwise length
 */
int64_t oscar_snac_read(const char *data, uint64_t size, oscar_snac *snac)
{
	assert(data != NULL);
	assert(snac != NULL);

	if (size <= OSCAR_SNAC_HEADER)
		LCH_ERROR(-1, "Insufficient data - no SNAC header");

	snac->type = ntohs(*((uint16_t *)&data[0]));
	snac->subtype = ntohs(*((uint16_t *)&data[2]));
	snac->flags = ntohs(*((uint16_t *)&data[4]));
	snac->reqid = ntohl(*((uint32_t *)&data[6]));

	size -= OSCAR_SNAC_HEADER;

	/* XXX - bleah, should in-place parse */
	LCH_CALLOC(snac->data, size, 1);
	memcpy(snac->data, &data[10], size);

	return size + OSCAR_SNAC_HEADER;
}

/*
 * oscar_snac_free - free a SNAC's internal data
 * in: oscar_snac struct
 * out: None
 */
void oscar_snac_free(oscar_snac *snac)
{
	assert(snac != NULL);

	LCH_FREE(snac->data);
}

/*
 * oscar_snac_create - create a SNAC structure
 * in: type, subtype, flags, reqid, data to send in SNAC frame, length
 * out: SNAC-framed data, size
 */
uint64_t oscar_snac_create(const char *data, uint16_t type, uint16_t subtype, uint16_t flags, uint32_t reqid, uint64_t size, char *out)
{
	assert(data != NULL);
	assert(out != NULL);
	assert(size);

	 *((uint16_t *)&out[0]) = htons(type);
	 *((uint16_t *)&out[2]) = htons(subtype);
	 *((uint16_t *)&out[4]) = htons(flags);
	 *((uint16_t *)&out[6]) = htonl(reqid);
	 memcpy(&out[10], data, size);

	 return OSCAR_SNAC_HEADER + size;
}


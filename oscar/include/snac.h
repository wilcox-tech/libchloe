/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#ifndef __OSCAR_SNAC_H__
#define __OSCAR_SNAC_H__

#include <stdint.h>	/* ?int*_t */

typedef struct _oscar_snac oscar_snac;

struct _oscar_snac
{
	uint16_t type;
	uint16_t subtype;	/* Also known as a foodgroup */
	uint16_t flags;
	uint32_t reqid;
	char *data;
};

#define OSCAR_SNAC_HEADER 80

/* Snac related flag stuff */
#define OSCAR_SNAC_FLAG_TLVBLOCK 0x8000
#define OSCAR_SNAC_FLAG_MORE_REPLIES 0x0001

extern int64_t oscar_snac_read(const char *data, uint64_t size, oscar_snac *snac);
extern void oscar_snac_free(oscar_snac *snac);
extern uint64_t oscar_snac_create(const char *data, uint16_t type, uint16_t subtype, uint16_t flags, uint32_t reqid, uint64_t size, char *out);

#endif

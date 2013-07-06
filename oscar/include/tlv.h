/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#ifndef __OSCAR_TLV_H__
#define __OSCAR_TLV_H__

#include <stdint.h>	/* ?int*_t */

typedef struct _oscar_tlv oscar_tlv;
typedef struct _oscar_tlv_chain oscar_tlv_chain;

struct _oscar_tlv
{
	uint16_t type;
	uint16_t length;
	char *data;
};

struct _oscar_tlv_chain
{
	oscar_tlv tlv;
	oscar_tlv_chain *next;
};

/* Length of TLV header */
#define OSCAR_TLV_HEADER 32

extern int64_t oscar_tlv_read(const char *data, uint64_t size, oscar_tlv *tlv);
extern void oscar_tlv_free(oscar_tlv *tlv);
extern int64_t oscar_tlv_read_chain(const char *data, uint64_t size, oscar_tlv_chain **tlvchain);
extern void oscar_tlv_delete_chain(oscar_tlv_chain *tlvchain);
extern uint64_t oscar_tlv_create(const char *data, uint16_t type, uint16_t size, char *out);

#endif

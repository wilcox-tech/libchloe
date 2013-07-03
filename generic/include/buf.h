/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#ifndef __LCH_BUF_H__
#define __LCH_BUF_H__

#include <stdint.h>	/* ?int*_t */
#include <assert.h>

struct _lch_buf
{
	char *data;		/* allocated buffer data */
	uint64_t len;		/* Length of data presently in the buffer */
	uint64_t maxlen;	/* what it says on the tin */
};

typedef struct _lch_buf lch_buf;

extern void lch_buf_init(lch_buf *buf, uint64_t maxlen);
extern void lch_buf_delete(lch_buf *buf);
extern uint64_t lch_buf_put(lch_buf *buf, char *data, uint64_t len);
extern uint64_t lch_buf_peek(lch_buf *buf, char *data, uint64_t len);
extern uint64_t lch_buf_get(lch_buf *buf, char *data, uint64_t len);

#endif

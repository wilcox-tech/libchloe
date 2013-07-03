/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#include <stdio.h>	/* fprintf, stderr, etc */
#include <string.h>	/* memmove */
#include <stdint.h>	/* ?int*_t, *INT*_MIN/MAX */
#include <stdlib.h>	/* exit, EXIT_FAILURE */
#include <assert.h>	/* figure it out yourself. */

#include "libchloe/include/buf.h"

/* 
 * Initalise an lch_buf
 * in: buffer ptr, max data len
 * out: nothing
 */
void lch_buf_init(lch_buf *buf, uint64_t maxlen)
{
	assert(buf != NULL);
	assert(maxlen);

	buf->maxlen = maxlen;

	LCH_CALLOC(buf->data, (size_t)maxlen, 1);
}

/*
 * Destroy an lch_buf
 * in: buffer ptr
 * out: nothing
 */
void lch_buf_delete(lch_buf *buf)
{
	buf->len = buf->maxlen = 0;

	LCH_FREE(buf->data);
}

/*
 * stuff data into an lch_buf
 *
 * NOTE: dest and src MUST NOT OVERLAP!
 *
 * in: buffer ptr, data, data length
 * out: length of data copied, -1 on other failure
 */
uint64_t lch_buf_put(lch_buf *buf, char *data, uint64_t len)
{
	assert(buf);
	assert(data != NULL);
	assert(len);

	/* Cap to remaining space if needed */
	if ((buf->maxlen - buf->len) >= len)
		len = buf->maxlen - buf->len;

	buf->len += len;
	memcpy(buf->data + buf->len, data, len);

	return len;
}

/*
 * Peek data from an lch_buf (doesn't consume)
 *
 * in: buffer ptr, data ptr, length to peek at
 * out: -2 on insufficient data, -1 on other error, 0 success
 */
uint64_t lch_buf_peek(lch_buf *buf, char *data, uint64_t len)
{
	assert(buf);
	assert(data != NULL);
	assert(len);

	if (buf->len < len)
		len = buf->len;

	memcpy(data, buf->data, len);

	return len;
}

/*
 * Pull data out of an lch_buf
 *
 * in: buffer ptr, data ptr, length to extract
 * out: length of data copied, -1 on other error
 */
uint64_t lch_buf_get(lch_buf *buf, char *data, uint64_t len)
{
	len = lch_buf_peek(buf, data, len);

	/* Move everything to the head */
	if (buf->len -= len)
		memmove(buf->data, buf->data + len, buf->len);

	return len;
}


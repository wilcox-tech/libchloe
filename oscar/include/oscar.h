/* Copyright (C) 2013 Wilcox Technologies, LLC. All rights Reserved.
 *
 * License information is contained in LICENSE, included with the distribution
 * of this software.
 */

#ifndef __OSCAR_OSCAR_H__
#define __OSCAR_OSCAR_H__

#include <stdlib.h>	/* EXIT_FAILURE, exit */
#include <stdio.h>	/* fprintf, stderr */
#include <stdint.h>	/* uint16_t */

struct _oscar
{
	uint16_t seq;
};

typedef struct _oscar oscar;

#endif

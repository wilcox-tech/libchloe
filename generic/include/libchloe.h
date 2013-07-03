#ifndef __LCH_LIBCHLOE_H__
#define __LCH_LIBCHLOE_H__

#include <stdio.h>	/* fprintf */
#include <stdlib.h>	/* calloc, exit, EXIT_FAILURE */

#define LCH_CALLOC(p, n, s) { if (((p) = calloc((n), (s))) == NULL) { fprintf(stderr, "Allocator failure"); exit(EXIT_FAILURE); } }
#define LCH_FREE(n) { free((n)); (n) = NULL; }
#define LCH_ERROR(v, ...) { fprintf(stderr, "In file %s line %d: ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); return (v); }

#endif

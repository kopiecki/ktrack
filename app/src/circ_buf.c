/**
 * @file   circ_buf.c
 * @brief  Trivial circural buffer implementation
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "circ_buf.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int circ_buf_init(struct circ_buf* const cb, size_t size)
{
	assert(cb != NULL);

	for (cb->mask = 1; cb->mask < size; cb->mask *= 2)
		;

	cb->x = calloc(cb->mask, sizeof(struct vec2));
	if (cb->x == NULL)
		return -ENOMEM;

	cb->mask--;
	cb->idx       = 0;
	cb->curr_size = 0;

	return 0;
}

void circ_buf_destroy(struct circ_buf* const cb)
{
	free(cb->x);
}

void circ_buf_push(struct circ_buf* const cb, struct vec2 v)
{
	assert(cb != NULL);

	cb->idx        = (cb->idx - 1) & cb->mask;
	cb->x[cb->idx] = v;

	if (cb->curr_size < circ_buf_capacity(cb))
		cb->curr_size++;
}

void circ_buf_reset(struct circ_buf* const cb)
{
	assert(cb != NULL);

	memset(cb->x, 0, circ_buf_curr_size(cb) * sizeof(struct vec2));
}

/**** END OF FILE ****/

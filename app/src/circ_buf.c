/*
 * @file   circ_buf.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "circ_buf.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int circ_buf_init(struct circ_buf* const cb, size_t size)
{
	assert(cb != NULL);

	for (cb->mask = 1; cb->mask < size; cb->mask *= 2)
		;

	cb->mask--;
	cb->idx = 0;

	cb->x = calloc(cb->mask + 1, sizeof(struct vec2));
	if (cb->x == NULL)
		return -ENOMEM;

	cb->curr_size = 0;

	return 0;
}

void circ_buf_destroy(struct circ_buf* const cb)
{
	free(cb->x);
}

void circ_buf_reset(struct circ_buf* const cb)
{
	assert(cb != NULL);

	memset(cb->x, 0, circ_buf_curr_size(cb) * sizeof(struct vec2));
}

/**** END OF FILE ****/

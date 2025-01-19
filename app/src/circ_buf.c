/*
 * @file   circ_buf.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "circ_buf.h"

#include <asm-generic/errno.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int circ_buf_init(struct circ_buf* const cb, struct vec2* const x, u32 size)
{
	assert(cb != NULL);

	for (cb->mask = 1; cb->mask < size; cb->mask *= 2)
		;

	cb->mask--;
	cb->idx = 0;

	if (x == NULL) {
		cb->x = calloc(cb->mask + 1, sizeof(struct vec2));
		if (cb->x == NULL)
			return -ENOMEM;

		cb->dalloc = true;
	}

	return 0;
}

void circ_buf_destroy(struct circ_buf* const cb)
{
	if (cb->dalloc)
		free(cb->x);
}

/**** END OF FILE ****/

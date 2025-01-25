/*
 * @file   circ_buf.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CIRC_BUF_H_INCLUDED_
#define CIRC_BUF_H_INCLUDED_

#include <stddef.h>

#include "common.h"

struct circ_buf {
	struct vec2* x;
	size_t mask;
	size_t idx;
	size_t curr_size;
};

int circ_buf_init(struct circ_buf* const cb, size_t size);
void circ_buf_reset(struct circ_buf* const cb);

void circ_buf_destroy(struct circ_buf* const cb);

static inline struct vec2 circ_buf_get(const struct circ_buf* const cb, size_t i)
{
	return cb->x[(cb->idx + i) & cb->mask];
}

static inline struct vec2 circ_buf_last(const struct circ_buf* const cb)
{
	return circ_buf_get(cb, 0);
}

static inline size_t circ_buf_capacity(const struct circ_buf* const cb)
{
	return cb->mask + 1;
}

static inline void circ_buf_push(struct circ_buf* const cb, struct vec2 v)
{
	cb->idx        = (cb->idx - 1) & cb->mask;
	cb->x[cb->idx] = v;

	if (cb->curr_size < circ_buf_capacity(cb))
		cb->curr_size++;
}

static inline size_t circ_buf_curr_size(const struct circ_buf* const cb)
{
	return cb->curr_size;
}

#endif /* CIRC_BUF_H_INCLUDED_ */

/**** END OF FILE ****/

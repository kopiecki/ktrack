/*
 * @file   circ_buf.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CIRC_BUF_H_INCLUDED_
#define CIRC_BUF_H_INCLUDED_

#include <stdbool.h>

#include "common.h"

struct circ_buf {
	struct vec2* x;
	u32 mask;
	u32 idx;
	bool dalloc;
};

int circ_buf_init(struct circ_buf* const cb, struct vec2* const x, u32 size);
void circ_buf_destroy(struct circ_buf* const cb);

static inline struct vec2 circ_buf_get(const struct circ_buf* const cb, u32 i)
{
	return cb->x[(cb->idx + i) & cb->mask];
}

static inline struct vec2 circ_buf_last(const struct circ_buf* const cb)
{
	return circ_buf_get(cb, 0);
}

static inline void circ_buf_push(struct circ_buf* const cb, struct vec2 v)
{
	cb->idx        = (cb->idx - 1) & cb->mask;
	cb->x[cb->idx] = v;
}

static inline u32 circ_buf_capacity(const struct circ_buf* const cb)
{
	return cb->mask + 1;
}

#endif /* CIRC_BUF_H_INCLUDED_ */

/**** END OF FILE ****/

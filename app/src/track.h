/*
 * @file   track.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TRACK_H_INCLUDED_
#define TRACK_H_INCLUDED_

#include "circ_buf.h"
#include "engine/common.h"
#include "engine/engine.h"
#include "engine/text.h"

struct track_cfg {
	const char* const name;
	SDL_Color drawing_col;
	const struct font_info* const text_font;
	struct vec2 text_pos;
	struct vec2 sample_dims;
	size_t cbuf_size;
};

struct track {
	const char* name;
	struct circ_buf cbuf;
	struct text text;
	struct rect sample;
};

int track_init(struct track* const tr, const struct track_cfg* const cfg);
void track_destroy(struct track* const tr);

int track_draw(struct track* const tr, struct engine* const e);
int track_report(struct track* const tr, struct engine* const e);

static inline void track_reset(struct track* const tr)
{
	circ_buf_reset(&tr->cbuf);
}

static inline void track_update(struct track* const tr, struct vec2 pos)
{
	circ_buf_push(&tr->cbuf, pos);
}

#endif /* TRACK_H_INCLUDED_ */

/**** END OF FILE ****/

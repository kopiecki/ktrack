/**
 * @file   track.c
 * @brief  Track representation
 * @author Daniel Kopiecki
 * @date   2025-01-26
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "track.h"

#include <assert.h>

#include "circ_buf.h"

#define TEXT_BUF_ADD_SIZE 15
#define TEXT_FMT_STR      "%s: [%d, %d]"

static inline int draw_sample(
	struct track* const tr, struct engine* const e, struct vec2 pos)
{
	tr->sample.dim.x = pos.x;
	tr->sample.dim.y = pos.y;

	return engine_draw_rect(e, &tr->sample);
}

int track_init(struct track* const tr, const struct track_cfg* const cfg)
{
	assert(tr != NULL);
	assert(cfg != NULL);

	int err = circ_buf_init(&tr->cbuf, cfg->cbuf_size);
	if (err < 0)
		return err;

	err = text_init(
		&tr->text, cfg->text_font, strlen(cfg->name) + TEXT_BUF_ADD_SIZE);
	if (err < 0)
		goto buf_destroy;

	tr->sample.col   = cfg->drawing_col;
	tr->sample.dim.w = cfg->sample_dims.x;
	tr->sample.dim.h = cfg->sample_dims.y;

	tr->text.col = cfg->drawing_col;
	text_set_pos(&tr->text, cfg->text_pos);

	tr->name = cfg->name;

	return 0;

buf_destroy:
	circ_buf_destroy(&tr->cbuf);

	return err;
}

void track_destroy(struct track* const tr)
{
	circ_buf_destroy(&tr->cbuf);
	text_destroy(&tr->text);
}

int track_draw(struct track* const tr, struct engine* const e)
{
	const size_t size = circ_buf_curr_size(&tr->cbuf);
	struct vec2 pos   = circ_buf_get(&tr->cbuf, 0);
	int err           = 0;

	for (size_t i = 1; i < size; i++) {
		err = draw_sample(tr, e, pos);
		if (err < 0)
			return err;

		pos = circ_buf_get(&tr->cbuf, i);
	}

	return 0;
}

int track_report(struct track* const tr, struct engine* const e)
{
	const struct vec2 last = circ_buf_last(&tr->cbuf);

	const int err =
		text_format(&tr->text, TEXT_FMT_STR, tr->name, last.x, last.y);
	if (err < 0)
		return err;

	return engine_draw_text(e, &tr->text);
}

/**** END OF FILE ****/

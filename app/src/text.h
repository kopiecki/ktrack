/*
 * @file   text.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TEXT_H_INCLUDED_
#define TEXT_H_INCLUDED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"

struct font_info {
	const char* const path;
	size_t size;
};

struct text {
	TTF_Font* font;
	SDL_Color col;
	SDL_Rect dim;
	char* buf;
};

int text_init(
	struct text* const t, const struct font_info* const font, char* const buf);
int text_load_font(struct text* const t, const struct font_info* const font);

static inline void text_set_pos(struct text* const t, struct vec2 pos)
{
	t->dim.x = pos.x;
	t->dim.y = pos.y;
}

static inline void text_set_col(struct text* const t, const SDL_Color* const col)
{
	t->col = *col;
}

#endif /* TEXT_H_INCLUDED_ */

/**** END OF FILE ****/

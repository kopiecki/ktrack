/*
 * @file   text.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "text.h"

#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <errno.h>

int text_init(
	struct text* const t, const struct font_info* const font, char* const buf)
{
	assert(t != NULL);

	int err = text_load_font(t, font);
	if (err < 0)
		return err;

	if (buf == NULL)
		return -ENOBUFS;

	t->buf = buf;

	return 0;
}

int text_load_font(struct text* const t, const struct font_info* const font)
{
	assert(t != NULL);
	assert(font != NULL);

	if (t->font != NULL)
		TTF_CloseFont(t->font);

	t->font = TTF_OpenFont(font->path, font->size);
	if (t->font == NULL)
		return -ENOMEM;

	return 0;
}

/**** END OF FILE ****/

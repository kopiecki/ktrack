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

int text_init(struct text* const t, const struct font* const font, size_t size)
{
	assert(t != NULL);

	t->buf = calloc(size, sizeof(char));
	if (t->buf == NULL)
		return -ENOMEM;

	t->size = size;

	t->font = TTF_OpenFont(font->path, (int)font->size);
	if (t->font == NULL)
		return -ENOMEM;

	return 0;
}

void text_destroy(struct text* const t)
{
	free(t->buf);
	TTF_CloseFont(t->font);
}

int text_format(struct text* const t, const char* const fmt, ...)
{
	va_list va;
	va_start(va, fmt);

	int num_chars = vsnprintf(t->buf, t->size, fmt, va);

	va_end(va);

	return num_chars;
}

/**** END OF FILE ****/

/**
 * @file   text.c
 * @brief  Aggregated renderable text information
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "engine/text.h"

#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <errno.h>

int text_init(struct text* const t, const struct font_info* const font, size_t size)
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

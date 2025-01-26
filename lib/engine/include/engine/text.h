/**
 * @file   text.h
 * @brief  Renderable text encapsulation
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef TEXT_H_INCLUDED_
#define TEXT_H_INCLUDED_

#include <SDL2/SDL_ttf.h>

#include "engine/vec2.h"

/**
 * @brief Aggregated font information
 */
struct font_info {
	const char* const path; /**< Path to .ttf font file*/
	size_t size;            /**< Font size*/
};

/**
 * @brief Renderable text representation
 */
struct text {
	TTF_Font* font; /**< SDL TTF font pointer*/
	SDL_Color col;  /**< Text color*/
	SDL_Rect dim;   /**< Rendered texture dimensions*/
	char* buf;      /**< Text buffer*/
	size_t size;    /**< Text buffer size*/
};

/**
 * @brief Initialize renderable text object
 *
 * @param[in] t      Instance pointer
 * @param[in] font   Aggregated font information
 * @param[in] size   Text buffer size
 * @return int       0 on success, negative error code otherwise
 */
int text_init(struct text* const t, const struct font_info* const font, size_t size);

/**
 * @brief Destroy text object
 *
 * @param[in] t     Instance pointer
 */
void text_destroy(struct text* const t);

/**
 * @brief Format text buffer
 *
 * @param[in] t       Instance pointer
 * @param[in] fmt     Format string
 * @param[in] ...     `printf` like variable argument lists
 * @return int        0 on success, negative error code otherwise
 */
int text_format(struct text* const t, const char* const fmt, ...);

/**
 * @brief Set text render position
 *
 * @param[in] t    Instance pointer
 * @param[in] pos  Position to set
 */
static inline void text_set_pos(struct text* const t, struct vec2 pos)
{
	t->dim.x = pos.x;
	t->dim.y = pos.y;
}

#endif /* TEXT_H_INCLUDED_ */

/**** END OF FILE ****/

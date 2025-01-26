/**
 * @file   rect.h
 * @brief  Aggregated renderable rectangle info
 * @author Daniel Kopiecki
 * @date   2025-01-26
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef RECT_H_INCLUDED_
#define RECT_H_INCLUDED_

#include <SDL2/SDL_rect.h>

/**
 * @brief Aggregated renderable rectangle object
 */
struct rect {
	SDL_Rect dim;  /**< Rectangle dimensions (w, h, x, y)*/
	SDL_Color col; /**< Rectangle color*/
};

#endif /* RECT_H_INCLUDED_ */

/**** END OF FILE ****/

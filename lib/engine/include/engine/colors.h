/**
 * @file   colors.h
 * @brief  Color constants
 * @author Daniel Kopiecki
 * @date   2025-01-26
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef COLORS_H_INCLUDED_
#define COLORS_H_INCLUDED_

#include <SDL2/SDL_pixels.h>

static const SDL_Color color_navy = {
	.r = 0,
	.g = 17,
	.b = 51,
	.a = 255,
};

static const SDL_Color color_blue = {
	.r = 0,
	.g = 0,
	.b = 255,
	.a = 255,
};

static const SDL_Color color_red = {
	.r = 255,
	.g = 0,
	.b = 0,
	.a = 255,
};

static const SDL_Color color_green = {
	.r = 0,
	.g = 255,
	.b = 0,
	.a = 255,
};

#endif /* COLORS_H_INCLUDED_ */

/**** END OF FILE ****/

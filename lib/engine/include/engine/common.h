/**
 * @file   common.h
 * @brief  Common SDL backend types and constants
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_INCLUDED_
#define COMMON_H_INCLUDED_

#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;

static const SDL_Color color_navy  = { .r = 0, .g = 17, .b = 51, .a = 255 };
static const SDL_Color color_blue  = { .r = 0, .g = 0, .b = 255, .a = 255 };
static const SDL_Color color_red   = { .r = 255, .g = 0, .b = 0, .a = 255 };
static const SDL_Color color_green = { .r = 0, .g = 255, .b = 0, .a = 255 };

/**
 * @brief Aggregated renderable rectangle object
 */
struct rect {
	SDL_Rect dim;  /**< Rectangle dimensions (w, h, x, y)*/
	SDL_Color col; /**< Rectangle color*/
};

/**
 * @brief Generic vector with two elements
 */
struct vec2 {
	i32 x; /**< X coordinate*/
	i32 y; /**< Y coordinate*/
};

/**
 * @brief Create vector
 *
 * @param[in] x            X coordinate
 * @param[in] y            Y coordinate
 * @return struct vec2
 */
static inline struct vec2 vec2_create(i32 x, i32 y)
{
	return (struct vec2) {
		.x = x,
		.y = y,
	};
}

/**
 * @brief Add vector to vector
 *
 * @param[in] lhs          Left hand side vector
 * @param[in] rhs          Right hand side vector
 * @return struct vec2 Result
 */
static inline struct vec2 vec2_add_v(const struct vec2 lhs, const struct vec2 rhs)
{
	return vec2_create(lhs.x + rhs.x, lhs.y + rhs.y);
}

/**
 * @brief Check if vectors are equal
 *
 * @param[in] lhs     Left hand side vector
 * @param[in] rhs     Right hand side vector
 * @return true   Equal
 * @return false  Not equal
 */
static inline bool vec2_eq(const struct vec2 lhs, const struct vec2 rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

#endif /* COMMON_H_INCLUDED_ */

/**** END OF FILE ****/

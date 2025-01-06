/*
 * @file   engine.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef ENGINE_H_INCLUDED_
#define ENGINE_H_INCLUDED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;

struct vec2 {
	i32 x;
	i32 y;
};

#define vec2_create(x, y)                                                      \
	(struct vec2)                                                              \
	{                                                                          \
		(x), (y)                                                               \
	}

static inline struct vec2 vec2_add_v(const struct vec2 lhs, const struct vec2 rhs)
{
	return vec2_create(lhs.x + rhs.x, lhs.y + rhs.y);
}

static inline bool vec2_eq(const struct vec2 lhs, const struct vec2 rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct rgba {
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};

struct rect {
	SDL_Rect dim;
	struct rgba col;
};

struct engine {
	SDL_Window* win;
	SDL_Renderer* rndr;
};

int engine_init(struct engine* const e, const char* name, struct vec2 size);
void engine_destroy(struct engine* const e);

int engine_poll_mouse(struct engine* const e, struct vec2* const corr);

int engine_draw_rect(struct engine* const e, const struct rect* const r);
void engine_wait(u32 ms);

#endif /* ENGINE_H_INCLUDED_ */

/**** END OF FILE ****/

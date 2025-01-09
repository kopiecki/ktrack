/*
 * @file   engine.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef ENGINE_H_INCLUDED_
#define ENGINE_H_INCLUDED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <stdatomic.h>
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

struct rect {
	SDL_Rect dim;
	SDL_Color col;
};

struct text {
	SDL_Rect dim;
	const char* msg;
	const char* font;
	SDL_Color col;
	u32 size;
};

static const SDL_Color color_blue = {
	.r = 0,
	.g = 17,
	.b = 51,
	.a = 0,
};

struct engine {
	SDL_Window* win;
	SDL_Renderer* rndr;
	SDL_Color bg_col;
};

int engine_init(struct engine* const e, const char* name, struct vec2 size);
void engine_destroy(struct engine* const e);

struct vec2 engine_poll_mouse(struct engine* const e, const atomic_bool* const quit);
SDL_KeyCode engine_poll_key(struct engine* const e, const atomic_bool* const quit);

int engine_draw_rect(struct engine* const e, const struct rect* const r);
int engine_draw_text(struct engine* const e, struct text* const t);

int engine_set_background(struct engine* const e, const SDL_Color* const col);
int engine_clear(struct engine* const e);

void engine_render(struct engine* const e);
void engine_wait(u32 ms);

#endif /* ENGINE_H_INCLUDED_ */

/**** END OF FILE ****/

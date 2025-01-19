/*
 * @file   engine.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef ENGINE_H_INCLUDED_
#define ENGINE_H_INCLUDED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#include "common.h"

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

static const SDL_Color color_navy  = { .r = 0, .g = 17, .b = 51, .a = 255 };
static const SDL_Color color_blue  = { .r = 0, .g = 0, .b = 255, .a = 255 };
static const SDL_Color color_red   = { .r = 255, .g = 0, .b = 0, .a = 255 };
static const SDL_Color color_green = { .r = 0, .g = 255, .b = 0, .a = 255 };

static inline SDL_Color set_opacity(SDL_Color col, u8 a)
{
	return (SDL_Color) {
		.r = col.r,
		.g = col.g,
		.b = col.b,
		.a = a,
	};
}

#define log_info(...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define log_warn(...) SDL_LogInfo(SDL_LOG_CATEGORY_WARNING, __VA_ARGS__)
#define log_err(...)  SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__)

struct engine {
	struct engine_ops {
		int (*on_key_press)(struct engine* const e, SDL_Keycode key);
		int (*on_mouse_press)(struct engine* const e, struct vec2 pos);
		int (*on_render)(struct engine* const e);
	} ops;
	SDL_Window* win;
	SDL_Renderer* rndr;
	SDL_Color bg_col;
	SDL_Event ev;
	u32 frame_delay_msec;
	bool running;
};

int engine_init(
	struct engine* const e,
	const char* name,
	struct vec2 size,
	const SDL_Color* const bg_col,
	u32 frame_delay_msec,
	const struct engine_ops* const ops);
void engine_destroy(struct engine* const e);

int engine_run(struct engine* const e);

int engine_clear(struct engine* const e);

int engine_draw_rect(struct engine* const e, const struct rect* const r);
int engine_draw_text(struct engine* const e, struct text* const t);

int engine_set_background(struct engine* const e, const SDL_Color* const col);
int engine_clear(struct engine* const e);

void engine_wait(u32 ms);

#endif /* ENGINE_H_INCLUDED_ */

/**** END OF FILE ****/

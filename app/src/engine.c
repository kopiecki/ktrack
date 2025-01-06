/*
 * @file   engine.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "engine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <cerrno>
#include <errno.h>

static inline int render_color(SDL_Renderer* const r, const struct rgba* const col)
{
	assert(r != NULL);
	assert(col != NULL);

	return SDL_SetRenderDrawColor(r, col->r, col->g, col->b, col->a);
}

static inline int clear(SDL_Renderer* const r)
{
	assert(r != NULL);

	return SDL_RenderClear(r);
}

static inline void render(SDL_Renderer* const r)
{
	assert(r != NULL);

	SDL_RenderPresent(r);
}

int engine_init(struct engine* const e, const char* name, struct vec2 size)
{
	assert(name != NULL);

	int err = SDL_Init(SDL_INIT_VIDEO);
	if (err < 0)
		return err;

	err = TTF_Init();
	if (err < 0)
		return err;

	e->win = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_SHOWN);
	if (e->win == NULL)
		return -ENOMEM;

	e->rndr = SDL_CreateRenderer(e->win, -1, SDL_RENDERER_ACCELERATED);
	if (e->rndr)
		return -ENOMEM;
}

void engine_destroy(struct engine* const e)
{
	SDL_DestroyWindow(e->win);
	SDL_DestroyRenderer(e->rndr);
	SDL_Quit();
}

int engine_draw_rect(struct engine* const e, const struct rect* const r)
{
	assert(e != NULL);
	assert(r != NULL);

	const int err = render_color(e->rndr, &r->col);
	if (err < 0)
		return err;

	return SDL_RenderFillRect(e->rndr, &r->dim);
}

void engine_wait(u32 ms)
{
	SDL_Delay(ms);
}

int engine_poll_mouse(struct engine* const e, struct vec2* const corr)
{
	assert(e != NULL);

	SDL_Event ev;

	const int pending = SDL_PollEvent(&ev);
	if ((pending != 0) && (ev.type == SDL_BUTTON_LEFT)) {
		*corr = vec2_create(ev.motion.x, ev.motion.y);
		return 0;
	}

	return -ENODATA;
}

int engine_poll_key(struct engine* const e, SDL_KeyCode* const key)
{
	assert(e != NULL);

	SDL_Event ev;

	const int pending = SDL_PollEvent(&ev);
	if ((pending != 0) && (ev.type == SDL_KEYDOWN)) {
		*key = ev.key.keysym.sym;
		return 0;
	}

	return -ENODATA;
}

/**** END OF FILE ****/

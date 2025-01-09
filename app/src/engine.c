/*
 * @file   engine.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "engine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>

void log_err(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, fmt, args);

	va_end(args);
}

void log_info(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, args);

	va_end(args);
}

static inline int set_render_color(SDL_Renderer* const r, const SDL_Color* const col)
{
	assert(r != NULL);
	assert(col != NULL);

	const int err = SDL_SetRenderDrawColor(r, col->r, col->g, col->b, col->a);
	if (err < 0)
		log_err("Could not set color [%s]", SDL_GetError());

	return err;
}

static inline int clear(SDL_Renderer* const r)
{
	assert(r != NULL);

	const int err = SDL_RenderClear(r);
	if (err < 0)
		log_err("Could not clear render [%s]", SDL_GetError());

	return err;
}

int engine_init(struct engine* const e, const char* name, struct vec2 size)
{
	assert(name != NULL);

	int err = SDL_Init(SDL_INIT_VIDEO);
	if (err < 0) {
		log_err("Could not init SDL [%s]", SDL_GetError());
		return err;
	}

	err = TTF_Init();
	if (err < 0) {
		log_err("Could not init TTF [%s]", SDL_GetError());
		return err;
	}

	e->win = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_SHOWN);
	if (e->win == NULL) {
		log_err("Could not create window [%s]", SDL_GetError());
		return -ENOMEM;
	}

	e->rndr = SDL_CreateRenderer(e->win, -1, SDL_RENDERER_ACCELERATED);
	if (e->rndr == NULL) {
		log_err("Could not create renderer [%s]", SDL_GetError());
		return -ENOMEM;
	}

	err = set_render_color(e->rndr, &e->bg_col);
	if (err < 0)
		return err;

	err = clear(e->rndr);
	if (err < 0)
		return err;

	log_info("Initialized SDL backend");

	return err;
}

void engine_destroy(struct engine* const e)
{
	SDL_DestroyWindow(e->win);
	SDL_DestroyRenderer(e->rndr);
	SDL_Quit();

	log_info("Deinitialized SDL backend");
}

int engine_draw_rect(struct engine* const e, const struct rect* const r)
{
	assert(e != NULL);
	assert(r != NULL);

	int err = set_render_color(e->rndr, &r->col);
	if (err < 0)
		return err;

	err = SDL_RenderFillRect(e->rndr, &r->dim);
	if (err < 0) {
		log_err("Could not fill rect [%s]", SDL_GetError());
		return err;
	}

	return 0;
}

int engine_draw_text(struct engine* const e, struct text* const t)
{
	assert(e != NULL);
	assert(t != NULL);

	TTF_Font* const font = TTF_OpenFont(t->font, t->size);
	if (font == NULL) {
		log_err("Could not open font [%s]", SDL_GetError());
		return -ENOMEM;
	}

	SDL_Surface* const surf = TTF_RenderText_Solid(font, t->msg, t->col);
	if (surf == NULL) {
		log_err("Could not open surf [%s]", SDL_GetError());
		return -ENOMEM;
	}

	SDL_Texture* const texture = SDL_CreateTextureFromSurface(e->rndr, surf);
	if (texture == NULL) {
		log_err("Could not create texture from surface [%s]", SDL_GetError());
		return -ENOMEM;
	}

	int err = SDL_QueryTexture(texture, NULL, NULL, &t->dim.w, &t->dim.h);
	if (err < 0)
		log_err("Could not query texture for size [%s]", SDL_GetError());

	return err;
}

void engine_wait(u32 ms)
{
	SDL_Delay(ms);
}

struct vec2 engine_poll_mouse(struct engine* const e, const atomic_bool* const quit)
{
	assert(e != NULL);

	SDL_Event ev;
	int pending = 0;

	while (!*quit) {
		pending = SDL_PollEvent(&ev);
		if (pending && (ev.type == SDL_MOUSEBUTTONDOWN))
			return vec2_create(ev.motion.x, ev.motion.y);
	}

	return vec2_create(-1, -1);
}

SDL_KeyCode engine_poll_key(struct engine* const e, const atomic_bool* const quit)
{
	assert(e != NULL);

	SDL_Event ev;
	int pending = 0;

	while (!*quit) {
		pending = SDL_PollEvent(&ev);
		if (pending && (ev.type == SDL_KEYDOWN))
			return ev.key.keysym.sym;
	}

	return SDLK_UNKNOWN;
}

int engine_set_background(struct engine* const e, const SDL_Color* const col)
{
	assert(e != NULL);
	assert(col != NULL);

	const int err = set_render_color(e->rndr, col);
	if (err < 0)
		return err;

	return clear(e->rndr);
}

void engine_render(struct engine* const e)
{
	assert(e != NULL);

	SDL_RenderPresent(e->rndr);
}

/**** END OF FILE ****/

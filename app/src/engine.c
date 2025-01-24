/*
 * @file   engine.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "engine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>

#define UNUSED(v)     (void)(v)
#define ARR_SIZE(arr) (int)(sizeof((arr)) / sizeof((arr)[0]))

static atomic_bool quit = false;

static void quit_handler(int signo)
{
	UNUSED(signo);

	quit = true;
}

static int quit_handler_init(void)
{
	static struct sigaction sa;
	sa.sa_handler = quit_handler;

	int err = sigfillset(&sa.sa_mask);
	if (err < 0)
		return err;

	const int sigs[] = { SIGINT, SIGTERM };
	for (int i = 0; i < ARR_SIZE(sigs); i++) {
		err = sigaction(sigs[i], &sa, NULL);
		if (err < 0)
			return err;
	}

	return 0;
}

static inline bool is_running(struct engine* const e)
{
	return !quit && e->running;
}

static inline int set_render_color(SDL_Renderer* const r, const SDL_Color* const col)
{
	assert(r != NULL);
	assert(col != NULL);

	const int err = SDL_SetRenderDrawColor(r, col->r, col->g, col->b, col->a);
	if (err < 0)
		log_err("Could not set color [%s]", SDL_GetError());

	return 0;
}

static inline SDL_KeyCode key_from_ev(const SDL_Event* const ev)
{
	return ev->key.keysym.sym;
}

static inline struct vec2 vec_from_ev(const SDL_Event* const ev)
{
	return vec2_create(ev->motion.x, ev->motion.y);
}

static int render(struct engine* const e)
{
	assert(e != NULL);

	int err = engine_set_background(e, &e->bg_col);
	if (err < 0)
		return err;

	err = e->ops.on_render(e);
	if (err < 0)
		return err;

	SDL_RenderPresent(e->rndr);

	return 0;
}

static inline bool is_keydown(const SDL_Event* const ev)
{
	return (ev->type == SDL_KEYDOWN);
}

static inline bool is_mousepress(const SDL_Event* const ev)
{
	return (
		(ev->type == SDL_MOUSEMOTION) && (ev->button.button == SDL_BUTTON_LEFT));
}

static int poll_event(struct engine* const e)
{
	assert(e != NULL);

	int err = 0;

	while (SDL_PollEvent(&e->ev)) {
		if (is_keydown(&e->ev))
			err = e->ops.on_key_press(e, key_from_ev(&e->ev));
		if (is_mousepress(&e->ev))
			err = e->ops.on_mouse_press(e, vec_from_ev(&e->ev));
		if (err < 0)
			return err;
	}

	return 0;
}

int engine_init(
	struct engine* const e,
	const char* const name,
	struct vec2 size,
	const SDL_Color* const bg_col,
	u32 frame_delay_msec,
	const struct engine_ops* const ops)
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

	err = SDL_SetRenderDrawBlendMode(e->rndr, SDL_BLENDMODE_BLEND);
	if (err < 0) {
		log_err("Could not set blend mode [%s]", SDL_GetError());
		return err;
	}

	err = set_render_color(e->rndr, &e->bg_col);
	if (err < 0)
		return err;

	err = engine_clear(e);
	if (err < 0)
		return err;

	if (ops == NULL) {
		log_err("SDL backend requires event handler structure");
		return -ENODATA;
	}

	e->ops              = *ops;
	e->bg_col           = *bg_col;
	e->frame_delay_msec = frame_delay_msec;

	log_info("Initialized SDL backend");

	return 0;
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

	SDL_Surface* const surf = TTF_RenderText_Solid(t->font, t->buf, t->col);
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
	if (err < 0) {
		log_err("Could not query texture for size [%s]", SDL_GetError());
		return err;
	}

	err = SDL_RenderCopy(e->rndr, texture, NULL, &t->dim);
	if (err < 0)
		log_err("Could not copy texture [%s]", SDL_GetError());

	return err;
}

void engine_wait(u32 ms)
{
	SDL_Delay(ms);
}

int engine_clear(struct engine* const e)
{
	assert(e != NULL);

	const int err = SDL_RenderClear(e->rndr);
	if (err < 0)
		log_err("Could not clear render [%s]", SDL_GetError());

	return err;
}

int engine_set_background(struct engine* const e, const SDL_Color* const col)
{
	assert(e != NULL);
	assert(col != NULL);

	const int err = set_render_color(e->rndr, col);
	if (err < 0)
		return err;

	return engine_clear(e);
}

int engine_run(struct engine* const e)
{
	assert(e != NULL);

	quit_handler_init();
	e->running = true;

	int err = 0;

	while (is_running(e)) {
		err = render(e);
		if (err < 0)
			return err;

		err = poll_event(e);
		if (err < 0)
			return err;

		engine_wait(e->frame_delay_msec);
	}

	engine_destroy(e);

	return 0;
}

/**** END OF FILE ****/

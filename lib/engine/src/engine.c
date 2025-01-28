/**
 * @file   engine.c
 * @brief  Stupid SDL backend
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "engine/engine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdatomic.h>

#include "engine/sig_handler.h"

static const int sigs[] = { SIGINT, SIGTERM };
static atomic_bool quit = false;

static void quit_handler(int signo)
{
	UNUSED(signo);

	quit = true;
}

static inline bool is_running(struct engine* const e)
{
	return !quit && e->running;
}

static inline int set_render_color(SDL_Renderer* const r, const SDL_Color* const col)
{
	assert(r != NULL);
	assert(col != NULL);

	return SDL_SetRenderDrawColor(r, col->r, col->g, col->b, col->a);
}

static inline SDL_KeyCode key_from_ev(const SDL_Event* const ev)
{
	return ev->key.keysym.sym;
}

static inline struct vec2 vec_from_ev(const SDL_Event* const ev)
{
	return vec2_create(ev->motion.x, ev->motion.y);
}

static inline int render_clear(struct engine* const e)
{
	assert(e != NULL);

	return SDL_RenderClear(e->rndr);
}

static inline void wait(u32 ms)
{
	SDL_Delay(ms);
}

static int set_background(struct engine* const e, const SDL_Color* const col)
{
	assert(e != NULL);
	assert(col != NULL);

	const int err = set_render_color(e->rndr, col);
	if (err < 0)
		return err;

	return render_clear(e);
}

static int render(struct engine* const e)
{
	assert(e != NULL);

	int err = set_background(e, &e->bg_col);
	if (err < 0)
		return err;

	err = e->ops->on_render(e);
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
			err = e->ops->on_key(e, key_from_ev(&e->ev));
		if (is_mousepress(&e->ev))
			err = e->ops->on_mouse(e, vec_from_ev(&e->ev));
		if (err < 0)
			return err;
	}

	return 0;
}

int engine_init(
	struct engine* const e,
	const char* const name,
	struct vec2 size,
	SDL_Color bg_col,
	u32 frame_delay_msec,
	const struct engine_ops* const ops)
{
	assert(name != NULL);

	if (ops == NULL)
		return -ENODATA;

	int err = SDL_Init(SDL_INIT_VIDEO);
	if (err < 0)
		goto backend_fail;

	err = TTF_Init();
	if (err < 0)
		goto backend_fail;

	e->win = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size.x,
		size.y,
		SDL_WINDOW_SHOWN);
	if (e->win == NULL) {
		err = -ENOMEM;
		goto backend_fail;
	}

	e->rndr = SDL_CreateRenderer(e->win, -1, SDL_RENDERER_ACCELERATED);
	if (e->rndr == NULL) {
		err = -ENOMEM;
		goto win_free;
	}

	err = SDL_SetRenderDrawBlendMode(e->rndr, SDL_BLENDMODE_BLEND);
	if (err < 0)
		goto rndr_free;

	err = set_render_color(e->rndr, &e->bg_col);
	if (err < 0)
		goto rndr_free;

	err = render_clear(e);
	if (err < 0)
		goto rndr_free;

	e->ops              = ops;
	e->bg_col           = bg_col;
	e->frame_delay_msec = frame_delay_msec;

	SDL_Log("Initialized SDL backend");

	return 0;

rndr_free:
	SDL_DestroyRenderer(e->rndr);
win_free:
	SDL_DestroyWindow(e->win);
backend_fail:
	SDL_Quit();

	return err;
}

void engine_destroy(struct engine* const e)
{
	SDL_DestroyWindow(e->win);
	SDL_DestroyRenderer(e->rndr);
	SDL_Quit();

	SDL_Log("Deinitialized SDL backend");
}

int engine_draw_rect(struct engine* const e, const struct rect* const r)
{
	assert(e != NULL);
	assert(r != NULL);

	int err = set_render_color(e->rndr, &r->col);
	if (err < 0)
		return err;

	return SDL_RenderFillRect(e->rndr, &r->dim);
}

int engine_draw_text(struct engine* const e, struct text* const t)
{
	assert(e != NULL);
	assert(t != NULL);

	SDL_Surface* const surf = TTF_RenderText_Solid(t->font, t->buf, t->col);
	if (surf == NULL)
		return -ENOMEM;

	SDL_Texture* const texture = SDL_CreateTextureFromSurface(e->rndr, surf);
	if (texture == NULL)
		return -ENOMEM;

	int err = SDL_QueryTexture(texture, NULL, NULL, &t->dim.w, &t->dim.h);
	if (err < 0)
		return err;

	return SDL_RenderCopy(e->rndr, texture, NULL, &t->dim);
}

int engine_run(struct engine* const e)
{
	assert(e != NULL);

	sig_handler_init(sigs, ARR_SIZE(sigs), quit_handler);

	e->running = true;
	int err    = 0;

	while (is_running(e)) {
		err = render(e);
		if (err < 0)
			return err;

		err = poll_event(e);
		if (err < 0)
			return err;

		wait(e->frame_delay_msec);
	}

	return 0;
}

/**** END OF FILE ****/

/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "circ_buf.h"
#include "common.h"
#include "engine.h"
#include "noise.h"

#define TRACK_SIZE (1 << 8)

static const struct vec2 win_size = { 1000, 1000 };
static const char* hack_regular =
	"/home/dk/p/git/sw/ktrack/fonts/Hack-Regular.ttf";

static struct circ_buf mouse_track;
static struct circ_buf kf_track;
static struct noise meas_noise;

static inline struct rect make_track_element(struct vec2 pos)
{
	return (struct rect) {
		.dim = { .w = 2, .h = 2, .x = pos.x, .y = pos.y },
		.col = color_red,
	};
}

struct vec2 make_noise(void)
{
	const i32 x = noise_get(&meas_noise);
	const i32 y = noise_get(&meas_noise);

	return vec2_create(x, y);
}

int handle_keypress(struct engine* const e, SDL_Keycode key)
{
	switch (key) {
	case SDLK_ESCAPE:
		e->running = false;
		break;
	case SDLK_c:
		return engine_set_background(e, &color_blue);
	default:
		break;
	}

	return 0;
}

int handle_mousepress(struct engine* const e, struct vec2 pos)
{
	SDL_Log("Press! [%d, %d]", pos.x, pos.y);
	circ_buf_push(&mouse_track, vec2_add_v(pos, make_noise()));

	return 0;
}

static int put_text(struct engine* const e)
{
	struct text t = {
		.dim  = { .h = 100, .w = 100, .x = 800, .y = 0 },
		.msg  = "Hello!",
		.font = hack_regular,
		.col  = color_red,
		.size = 50,
	};

	return engine_draw_text(e, &t);
}

int on_render(struct engine* const e)
{
	for (u32 i = 0; i < TRACK_SIZE; i++) {
		const struct vec2 pos = circ_buf_get(&mouse_track, i);
		const struct rect r   = make_track_element(pos);
		const int err         = engine_draw_rect(e, &r);
		if (err < 0)
			return err;
	}

	put_text(e);

	return 0;
}

static struct engine e;

static const struct engine_ops ops = {
	.on_key_press   = handle_keypress,
	.on_mouse_press = handle_mousepress,
	.on_render      = on_render,
};

int main(void)
{
	int err = engine_init(&e, "ktrack", win_size, &color_blue, &ops);
	if (err < 0)
		goto engine_destroy;

	err = circ_buf_init(&mouse_track, NULL, TRACK_SIZE);
	if (err < 0)
		goto engine_destroy;

	err = circ_buf_init(&kf_track, NULL, TRACK_SIZE);
	if (err < 0)
		goto mouse_track_destroy;

	err = noise_init(&meas_noise, 5.0);
	if (err < 0)
		goto kf_track_destroy;

	err = engine_run(&e);

	noise_destroy(&meas_noise);

mouse_track_destroy:
	circ_buf_destroy(&mouse_track);
kf_track_destroy:
	circ_buf_destroy(&kf_track);
engine_destroy:
	engine_destroy(&e);

	return err;
}

/**** END OF FILE ****/

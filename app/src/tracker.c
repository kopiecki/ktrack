/*
 * @file   tracker.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "tracker.h"

#include <assert.h>

#include "circ_buf.h"
#include "common.h"
#include "config.h"
#include "engine.h"
#include "kf.h"
#include "rng.h"
#include "text.h"

#define child_of(base) (struct tracker*)(base)

static inline int fmt_txt_buf(char* const buf, const char* const str, struct vec2 pos)
{
	return snprintf(buf, MSG_SIZE, "%s[%d, %d]", str, pos.x, pos.y);
}

static inline struct rect make_sample(struct vec2 pos, SDL_Color col)
{
	return (struct rect) {
		.dim = {
			.w = sample_dims,
			.h = sample_dims,
			.x = pos.x,
			.y = pos.y,
		},
		.col = col,
	};
}

static int handle_keypress(struct engine* const e, SDL_Keycode key)
{
	switch (key) {
	case SDLK_ESCAPE:
		e->running = false;
		break;
	default:
		break;
	}

	return 0;
}

static struct vec2 take_meas(
	struct vec2 pos,
	struct circ_buf* const meas_track,
	const struct rng_gauss* const rng)
{
	const i32 xn = (i32)rng_gauss_get(rng);
	const i32 yn = (i32)rng_gauss_get(rng);

	const struct vec2 noise = vec2_create(xn, yn);
	const struct vec2 meas  = vec2_add_v(pos, noise);

	circ_buf_push(meas_track, meas);

	return meas;
}

static int update_kf(
	struct vec2 meas,
	struct circ_buf* const pred_track,
	struct kf* const kf,
	double* const x,
	double* const z,
	const double* const u)
{
	const int err = kf_predict(kf, u);
	if (err < 0)
		return err;

	kf_get(kf, x);

	const struct vec2 pred = vec2_create((i32)x[0], (i32)x[1]);
	circ_buf_push(pred_track, pred);

	z[0] = meas.x;
	z[1] = meas.y;

	return kf_update(kf, z);
}

static int handle_mousepress(struct engine* const e, struct vec2 pos)
{
	struct tracker* const tr = child_of(e);

	const struct vec2 meas = take_meas(pos, &tr->meas_track, &tr->meas_noise);

	return update_kf(meas, &tr->pred_track, &tr->kf, tr->x, tr->z, tr->u);
}

static int draw_track(
	struct engine* const e,
	const struct circ_buf* const track,
	u32 idx,
	SDL_Color col)
{
	assert(e != NULL);
	assert(track != NULL);

	const struct vec2 pos    = circ_buf_get(track, idx);
	const struct rect sample = make_sample(pos, col);

	return engine_draw_rect(e, &sample);
}

static int update_text(
	struct engine* const e,
	struct text* const t,
	const char* const str,
	struct vec2 pos)
{
	int num_chars = fmt_txt_buf(t->buf, str, pos);
	if (num_chars == 0)
		return -ENODATA;

	return engine_draw_text(e, t);
}

static int on_render(struct engine* const e)
{
	struct tracker* const tr = child_of(e);

	for (u32 i = 0; i < circ_buf_size; i++) {
		int err = draw_track(e, &tr->meas_track, i, set_opacity(m_col, 100));
		if (err < 0)
			return err;

		err = draw_track(e, &tr->pred_track, i, kf_col);
		if (err < 0)
			return err;
	}

	int err =
		update_text(e, &tr->meas_text, meas_str, circ_buf_last(&tr->meas_track));
	if (err < 0)
		return err;

	return update_text(
		e, &tr->pred_text, pred_str, circ_buf_last(&tr->pred_track));
}

static const struct engine_ops ops = {
	.on_render      = on_render,
	.on_key_press   = handle_keypress,
	.on_mouse_press = handle_mousepress,
};

int tracker_run(struct tracker* const tr)
{
	assert(tr != NULL);

	struct engine* const e = (struct engine*)tr;

	int err = engine_init(e, name, win_size, &bg_col, frame_delay_msec, &ops);
	if (err < 0)
		return err;

	err = text_init(&tr->meas_text, &hack_regular_25, tr->meas_buf);
	if (err < 0)
		return err;

	text_set_col(&tr->meas_text, &color_red);
	text_set_pos(&tr->meas_text, vec2_create(win_size.x - 350, 0));

	err = text_init(&tr->pred_text, &hack_regular_25, tr->pred_buf);
	if (err < 0)
		return err;

	text_set_col(&tr->pred_text, &color_green);
	text_set_pos(&tr->pred_text, vec2_create(win_size.x - 350, 25));

	err = circ_buf_init(&tr->meas_track, NULL, circ_buf_size);
	if (err < 0)
		return err;

	err = circ_buf_init(&tr->pred_track, NULL, circ_buf_size);
	if (err < 0)
		return err;

	err = rng_gauss_init(&tr->meas_noise, noise_stddev, noise_mean);
	if (err < 0)
		return err;

	err = kf_init(&tr->kf, f, h, q, r, b, STATE_SIZE, MEAS_SIZE);
	if (err < 0)
		return err;

	return engine_run(e);
}

void tracker_destroy(struct tracker* const tr)
{
	circ_buf_destroy(&tr->meas_track);
	circ_buf_destroy(&tr->pred_track);
	rng_gauss_destroy(&tr->meas_noise);
	kf_destroy(&tr->kf);
}

/**** END OF FILE ****/

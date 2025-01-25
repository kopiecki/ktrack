/*
 * @file   app.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "app.h"

#include <assert.h>

#include "common.h"
#include "config.h"
#include "engine.h"
#include "kf.h"
#include "rng.h"
#include "track.h"

#define child_of(base) (struct app*)(base)

static struct vec2 take_meas(
	struct vec2 pos,
	struct track* const meas_track,
	const struct rng_gauss* const rng)
{
	const i32 xn = (i32)rng_gauss_get(rng);
	const i32 yn = (i32)rng_gauss_get(rng);

	const struct vec2 noise = vec2_create(xn, yn);
	const struct vec2 meas  = vec2_add_v(pos, noise);

	track_update(meas_track, meas);

	return meas;
}

static int update_kf(
	struct vec2 meas,
	struct track* const pred_track,
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

	track_update(pred_track, pred);

	z[0] = meas.x;
	z[1] = meas.y;

	return kf_update(kf, z);
}

static int handle_mousepress(struct engine* const e, struct vec2 pos)
{
	struct app* const ctx = child_of(e);

	const struct vec2 meas =
		take_meas(pos, &ctx->tracks[track_tp_meas], &ctx->meas_noise);

	return update_kf(
		meas, &ctx->tracks[track_tp_pred], &ctx->kf, ctx->x, ctx->z, ctx->u);
}

static int handle_keypress(struct engine* const e, SDL_Keycode key)
{
	struct app* const ctx = child_of(e);

	switch (key) {
	case SDLK_ESCAPE:
		e->running = false;
		break;
	case SDLK_c:
		kf_reset(&ctx->kf);
		for (u8 i = 0; i < num_tracks; i++)
			track_reset(&ctx->tracks[i]);
		break;
	default:
		break;
	}

	return 0;
}

static int on_render(struct engine* const e)
{
	struct app* const ctx = child_of(e);

	for (u8 i = 0; i < num_tracks; i++) {
		int err = track_draw(&ctx->tracks[i], e);
		if (err < 0)
			return err;

		err = track_report(&ctx->tracks[i], e);
		if (err < 0)
			return err;
	}

	return 0;
}

static const struct engine_ops ops = {
	.on_render      = on_render,
	.on_key_press   = handle_keypress,
	.on_mouse_press = handle_mousepress,
};

int app_run(struct app* const ctx)
{
	assert(ctx != NULL);

	struct engine* const e = (struct engine*)ctx;

	int err = engine_init(e, app_name, win_size, &bg_col, frame_delay_msec, &ops);
	if (err < 0)
		return err;

	for (u8 i = 0; i < num_tracks; i++) {
		err = track_init(&ctx->tracks[i], &track_cfgs[i]);
		if (err < 0)
			return err;
	}

	err = rng_gauss_init(&ctx->meas_noise, noise_stddev, noise_mean);
	if (err < 0)
		return err;

	err = kf_init(&ctx->kf, f, h, q, r, b, STATE_SIZE, MEAS_SIZE);
	if (err < 0)
		return err;

	return engine_run(e);
}

void app_destroy(struct app* const ctx)
{
	for (u8 i = 0; i < num_tracks; i++)
		track_destroy(&ctx->tracks[i]);

	rng_gauss_destroy(&ctx->meas_noise);
	kf_destroy(&ctx->kf);
}

/**** END OF FILE ****/

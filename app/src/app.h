/*
 * @file   app.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef APP_H_INCLUDED_
#define APP_H_INCLUDED_

#include <SDL2/SDL_pixels.h>

#include "circ_buf.h"
#include "config.h"
#include "engine.h"
#include "kf.h"
#include "rng.h"
#include "track.h"

struct app {
	struct engine base;
	struct track tracks[num_tracks];
	struct rng_gauss meas_noise;
	struct kf kf;
	double x[STATE_SIZE];
	double u[STATE_SIZE];
	double z[MEAS_SIZE];
};

int app_run(struct app* const ctx);
void app_destroy(struct app* const ctx);

#endif /* APP_H_INCLUDED_ */

/**** END OF FILE ****/

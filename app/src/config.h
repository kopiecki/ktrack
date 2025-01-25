/*
 * @file   cfg.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CFG_H_INCLUDED_
#define CFG_H_INCLUDED_

#include <gsl/gsl_matrix.h>

#include "common.h"
#include "engine.h"
#include "track.h"

static const char* const app_name = "ktrack";
static const u32 frame_delay_msec = 25;
static const struct vec2 win_size = { 1000, 1000 };
static const SDL_Color bg_col     = color_navy;

static const struct font hack_regular_25 = {
	.path = "fonts/Hack-Regular.ttf",
	.size = 25,
};

enum track_tp {
	track_tp_meas = 0,
	track_tp_pred,
	num_tracks,
};

static const struct track_cfg track_cfgs[] = {
	[track_tp_meas] = {
		.name        = "measurement",
		.drawing_col = color_red,
		.text_font   = &hack_regular_25,
		.text_pos    = { .x = 600, .y = 0 },
		.sample_dims = { .x = 2, .y = 2, },
		.cbuf_size   = (1 << 8),
	},
	[track_tp_pred] = {
		.name        = "prediction",
		.drawing_col = color_green,
		.text_font   = &hack_regular_25,
		.text_pos    = { .x = 614, .y = 25 },
		.sample_dims = { .x = 2, .y = 2, },
		.cbuf_size   = (1 << 8),
	},
};

static const double noise_stddev = 20.0;
static const double noise_mean   = 0.0;

#define MSG_SIZE 32

/**
 * @brief Number of states (x vector size)
 */
#define STATE_SIZE 4

/**
 * @brief Number of measurements (z vector size)
 */
#define MEAS_SIZE 2

// clang-format off

/**
 * @brief State transition model (STATE_SIZE x STATE_SIZE)
 */
static const double f[] = {
	1.0, 0.0, 1.0, 0.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0,
};

/**
 * @brief Observation model (MEAS_SIZE x STATE_SIZE)
 */
static const double h[] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
};

/**
 * @brief Process noise covariance (STATE_SIZE x STATE_SIZE)
 */
static const double q[] = {
	0.003, 0.0, 0.0, 0.0,
	0.0, 0.003, 0.0, 0.0,
	0.0, 0.0, 0.003, 0.0,
	0.0, 0.0, 0.0, 0.003,
};

/**
 * @brief Observation noise covariance (MEAS_SIZE x MEAS_SIZE)
 */
static const double r[] = {
	50.0 * 50.0, 0.0,
	0.0, 50.0 * 50.0,
};

/**
 * @brief Control-input model (STATE_SIZE x STATE_SIZE)
 */
static const double b[] = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
};

// clang-format on

#endif /* CFG_H_INCLUDED_ */

/**** END OF FILE ****/

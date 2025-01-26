/**
 * @file   config.h
 * @brief  Configuration file
 * @author Daniel Kopiecki
 * @date   2025-01-26
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef CONFIG_H_INCLUDED_
#define CONFIG_H_INCLUDED_

#include "engine/colors.h"
#include "track.h"

/**
 * @brief Application name
 */
#define APP_NAME "ktrack"

/**
 * @brief Delay in event/render loop
 */
#define FRAME_DELAY_MSEC 25U

/**
 * @brief Main window size
 */
static const struct vec2 win_size = {
	1000,
	1000,
};

/**
 * @brief Main window background color
 */
static const SDL_Color bg_col = color_navy;

/**
 * @brief Aggregated used font information
 */
static const struct font_info hack_regular_25 = {
	.path = "fonts/Hack-Regular.ttf",
	.size = 25,
};

/**
 * @brief Type of tracks
 */
enum track_tp {
	TRACK_MEAS = 0, /**< Measurement track */
	TRACK_PRED,     /**< Prediction track*/
	NUM_TRACKS,     /**< Enum guard*/
};

/**
 * @brief Tracks configuration
 */
static const struct track_cfg track_cfgs[] = {
	[TRACK_MEAS] = {
		.name        = "measurement",
		.drawing_col = color_red,
		.text_font   = &hack_regular_25,
		.text_pos    = { .x = 600, .y = 0 },
		.sample_dims = { .x = 2, .y = 2, },
		.cbuf_size   = (1 << 8),
	},
	[TRACK_PRED] = {
		.name        = "prediction",
		.drawing_col = color_green,
		.text_font   = &hack_regular_25,
		.text_pos    = { .x = 614, .y = 25 },
		.sample_dims = { .x = 2, .y = 2, },
		.cbuf_size   = (1 << 8),
	},
};

/**
 * @brief Measurement noise standard deviation
 */
#define MEAS_NOISE_STDDEV 20.0

/**
 * @brief Measurement noise mean
 */
#define MEAS_NOISE_MEAN 0.0

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

#endif /* CONFIG_H_INCLUDED_ */

/**** END OF FILE ****/

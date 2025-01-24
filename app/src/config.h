/*
 * @file   cfg.h
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef CFG_H_INCLUDED_
#define CFG_H_INCLUDED_

#include <SDL2/SDL_pixels.h>
#include <complex.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_matrix_double.h>
#include <stdbool.h>

#include "engine.h"

static const char* name           = "ktrack";
static const u32 frame_delay_msec = 25;
static const u32 circ_buf_size    = (1 << 8);
static const struct vec2 win_size = { 1000, 1000 };
static const SDL_Color bg_col     = color_navy;
static const SDL_Color kf_col     = color_green;
static const SDL_Color m_col      = color_red;
static const double noise_stddev  = 20.0;
static const double noise_mean    = 0.0;
static const u8 sample_dims       = 2;
static const char* meas_str       = "measurement: ";
static const char* pred_str       = "prediction:  ";

static const struct font_info hack_regular_25 = {
	.path = "/home/dk/p/git/sw/ktrack/fonts/Hack-Regular.ttf",
	.size = 25,
};

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

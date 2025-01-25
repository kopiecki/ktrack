/**
 * @file   app.h
 * @brief  Kalman tracker application
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef APP_H_INCLUDED_
#define APP_H_INCLUDED_

#include "config.h"
#include "dsp/kf.h"
#include "dsp/rng.h"
#include "track.h"

/**
 * @brief Application state
 */
struct app {
	struct engine base;              /**< Base (parent) struct*/
	struct track tracks[NUM_TRACKS]; /**< Measurement and prediction tracks*/
	struct rng_gauss meas_noise;     /**< Measurement noise*/
	struct kf kf;                    /**< Kalman filter instance*/
	double x[STATE_SIZE];            /**< State vector*/
	double u[STATE_SIZE];            /**< Control-input vector*/
	double z[MEAS_SIZE];             /**< Measurement vector*/
};

/**
 * @brief Run application
 * @details On exit, application destroy itself
 *
 * @param[in] ctx Instance pointer
 * @return int    0 on success, negative error code otherwise
 */
int app_run(struct app* const ctx);

/**
 * @brief Destroy application
 *
 * @param[in] ctx Instance pointer
 */
void app_destroy(struct app* const ctx);

#endif /* APP_H_INCLUDED_ */

/**** END OF FILE ****/

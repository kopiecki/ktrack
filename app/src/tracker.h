/*
 * @file   tracker.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef TRACKER_H_INCLUDED_
#define TRACKER_H_INCLUDED_

#include "circ_buf.h"
#include "config.h"
#include "engine.h"
#include "kf.h"
#include "rng.h"

struct tracker {
	struct engine base;
	struct circ_buf meas_track;
	struct circ_buf pred_track;
	struct rng_gauss meas_noise;
	struct kf kf;

	char meas_buf[MSG_SIZE];
	struct text meas_text;

	char pred_buf[MSG_SIZE];
	struct text pred_text;

	double x[STATE_SIZE];
	double u[STATE_SIZE];
	double z[MEAS_SIZE];
};

int tracker_run(struct tracker* const tr);
void tracker_destroy(struct tracker* const tr);

#endif /* TRACKER_H_INCLUDED_ */

/**** END OF FILE ****/

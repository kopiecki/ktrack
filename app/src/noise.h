/*
 * @file   noise.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef NOISE_H_INCLUDED_
#define NOISE_H_INCLUDED_

#include <gsl/gsl_rng.h>

struct noise {
	double stddev;
	gsl_rng* rng;
};

int noise_init(struct noise* const n, double stddev);
void noise_destroy(struct noise* const n);

double noise_get(struct noise* const n);

#endif /* NOISE_H_INCLUDED_ */

/**** END OF FILE ****/
